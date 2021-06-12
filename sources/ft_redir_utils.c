#include "minishell.h"

t_way	trim_redir(char *cmd, int *i)
{
	t_way	redirT;

	if (cmd[*i] == '>' && cmd[*i + 1] == '>')
	{
		*i = *i + 2;
		redirT = OUT_OUT;
	}
	else if (cmd[*i] == '>')
	{
		*i = *i + 1;
		redirT = OUT;
	}
	else if (cmd[*i] == '<' && cmd[*i + 1] == '<')
	{
		*i = *i + 2;
		redirT = IN_IN;
	}
	else if (cmd[*i] == '<')
	{
		*i = *i + 1;
		redirT = IN;
	}
	while (cmd[*i] == ' ' || cmd[*i] == '\t')
		*i = *i + 1;
	return (redirT);
}

t_bool	is_delimiter(char c)
{
	if (c == '>' || c == '<' || c == ' ' || c == '\n' || c == '\t')
		return (TRUE);
	else
		return (FALSE);
}

t_bool	is_redirection(char c)
{
	if (c == '<' || c == '>')
		return (TRUE);
	else
		return (FALSE);
}
