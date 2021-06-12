#include "minishell.h"

int	ft_free_strs(char **strs)
{
	int	i;

	i = -1;
	while (strs && strs[++i])
		free(strs[i]);
	free(strs);
	return (0);
}

void	ft_free_redir(void *content)
{
	free(((t_redir *)content)->filename);
	free(content);
}

void	ft_free_node(void *_node)
{
	t_btree	*node;

	node = _node;
	ft_lstclear(&(node->redir), ft_free_redir);
	free(node->cmd);
	ft_free_strs(node->argv);
	free(node->buff);
	free(node->delimiter);
	free(node);
}

void	ft_free_sets(void *content)
{
	free(((t_var *)content)->key);
	free(((t_var *)content)->value);
	free(content);
}

void	ft_free(t_btree *root, t_dico *dico)
{
	btree_apply_suffix(root, ft_free_node);
	ft_lstclear(&(dico->sets), ft_free_sets);
	ft_free_strs(dico->envp);
}

char	*_strerror(int _errno)
{
	static char	*strs_error[] = {
		NULL,
		"minishell: parse error near unexpected token",
		"minishell: multilines command are not supported",
		"minishell: ",
		"minishell: export: ",
		"minishell: unset: ",
		"minishell: cd: ",
		"minishell: exit: ",
		"minishell: echo: ",
		"minishell: pwd: ",
	};

	if (_errno > ELAST)
		return (strs_error[_errno % EOFFSET]);
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

	return (ft_error((const char *[]){_strerror(EPARSE),
			strs_error[t], NULL}, FALSE));
}

int	ft_error(const char *msg[], t_bool is_fatal)
{
	int	i;

	i = -1;
	if (msg)
		while (msg[++i])
			write(STDERR_FILENO, msg[i], ft_strlen(msg[i]));
	if (is_fatal)
	{
		ft_free(g_minishell.root, g_minishell.dico);
		exit(EXIT_FAILURE);
	}
	else
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
		ft_error((const char *[]){_strerror(EPARSE),
			" `", error, "'\n", NULL}, FALSE);
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
