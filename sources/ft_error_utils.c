#include "minishell.h"

char	*_strerror(int _errno)
{
	static char	*strs_error[] = {
		NULL,
		"minishell: syntax error near unexpected token",
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

int	pick_error(t_type t)
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
