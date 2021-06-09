#include "minishell.h"

int	ft_isvalid_id(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (i == 0 && !ft_isalpha(str[i]) && str[i] != '_')
			return 0;
		if (i > 0 && !ft_isalnum(str[i]) && str[i] != '_')
			return 0;
	}
	return 1;
}

int ft_unset(t_btree *node, t_dico *dico)
{
	char **argv;
	int code_return;

	code_return = 0;
	argv = node->argv + 1;
	if (!(*argv))
		return (code_return);
	while (*argv)
	{
		if (!ft_isvalid_id(*argv))
		{
			ft_error((const char *[]){_strerror(EEXPORT) ,"`",*argv ,"': not a valid identifier\n", NULL});
			code_return = 1;
		}
		else
			ft_rm_dico_var(*argv, dico);
		argv++;
	}
	return (code_return);
}
