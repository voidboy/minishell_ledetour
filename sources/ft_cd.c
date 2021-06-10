#include "minishell.h"

int ft_cd(t_btree *node, t_dico *dico)
{
	char *path;
	char *oldpwd;

	(void)dico;
	if (!node->argv[1])
		path = ft_get_dico_value("HOME", dico);
	else
		path = node->argv[1];
	if (chdir(path) == -1)
	{
		ft_error((const char *[]){_strerror(EEMPTY), "cd: ",
			path, ": ", _strerror(errno), "\n", NULL}, FALSE);
		return (1);
	}
	else 
	{
		oldpwd = ft_get_dico_value("PWD", dico);
		ft_set_dico_value(ft_strdup("OLDPWD"), ft_strdup(oldpwd), GLOBAL, dico);
		ft_set_dico_value(ft_strdup("PWD"), ft_strdup(path), GLOBAL, dico);

		return (0);
	}
}
