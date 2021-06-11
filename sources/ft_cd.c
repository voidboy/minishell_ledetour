#include "minishell.h"

static char *ft_get_old(t_dico *dico)
{	
	char *path;

	path = ft_get_dico_value("OLDPWD", dico);
	if (!path)
		ft_error((const char *[]){_strerror(ECD),
			"OLDPWD not set\n", NULL}, FALSE);
	return (path);

}

static char *ft_get_pwd(t_dico *dico)
{
	char *path;

	path = ft_get_dico_value("HOME", dico);
	if (!path)
		ft_error((const char *[]){_strerror(ECD),
			"HOME not set\n", NULL}, FALSE);
	return (path);

}

int ft_cd(t_btree *node, t_dico *dico)
{
	char *path;
	char *oldpwd;

	if (!node->argv[1] || !ft_strcmp(node->argv[1], "-"))
	{
		if (!node->argv[1])
			path = ft_get_pwd(dico);
		else 
			path = ft_get_old(dico);
		if (!path)
			return (1);
	}
	else
		path = node->argv[1];
	if (chdir(path) == -1)
	{
		ft_error((const char *[]){_strerror(ECD),
			path, ": ", _strerror(errno), "\n", NULL}, FALSE);
		return (1);
	}
	else 
	{
		oldpwd = ft_get_dico_value("PWD", dico);
		printf("OLD %s - PATH %s\n", oldpwd, path);
		ft_set_dico_value(ft_strdup("OLDPWD"), ft_strdup(oldpwd), GLOBAL, dico);
		ft_set_dico_value(ft_strdup("PWD"), getcwd(NULL, 0), GLOBAL, dico);
		return (0);
	}
}
