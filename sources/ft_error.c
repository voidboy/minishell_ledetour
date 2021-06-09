#include "minishell.h"

char	*_strerror(int _errno)
{
	static char	*strs_error[] = {
		NULL,
		"minishell: parse error near unexpected token",
		"minishell: multilines command are not supported",
		"minishell: ",
		"minishell: export: ",
		"minishell: unset: ",
	};

	if (_errno > ELAST)
		return (strs_error[_errno % ELAST]);
	else
		return (strerror(_errno));
}

static int	pick_error(t_type t)
{
	static char	*strs_error[] = {
		" `;'\n",
		" `&&'\n",
		" `||'\n",
		" `|'\n",
		NULL,
		" `newline'\n",
	};

	return (ft_error(EPARSE, (const char *[]){strs_error[t], NULL}));
}

int	ft_error(int _errno, const char *msg[])
{
	int 	i;
	char	*err_msg;

	if (_errno > ELAST)
		err_msg = _strerror(_errno);
	else 
		err_msg = strerror(_errno);
	write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	i = -1;
	if (msg)
		while (msg[++i])
			write(STDERR_FILENO, msg[i], ft_strlen(msg[i]));
	return (ERROR);
}

void	parse_error(t_btree *node, char last)
{
	char	error[2];

	if (last == '\n')
		pick_error(NEWLINE);
	else if (last)
	{
		error[0] = last;
		error[1] = '\0';
		ft_error(EPARSE, (const char *[]){" `", error, "'\n", NULL});
	}
	else if (node->side == LEFT)
		pick_error(node->parent->type);
	else if (node->side == RIGHT)
		pick_error(node->parent->parent->type);
}

int	check_left(t_btree *node)
{
	if (!node->left)
		return (pick_error(node->type));
	else 
		return (SUCCESS);
}

int	check_right(t_btree *node)
{
	if ((node->right && node->right->type == NEWLINE)
		|| (!node->right && !node->parent))
		return (pick_error(NEWLINE));
	else if (!node->right)
		return (pick_error(node->parent->type));
	else 
		return (SUCCESS);
}
