#include "minishell.h"

int ft_cd(t_btree *node, t_dico *dico)
{
	char *path;

	(void)dico;
	if (!node->argv[1])
		path = ft_get_dico_value("HOME", dico);
	else
		path = node->argv[1];
	if (chdir(path) == -1)
	{
		ft_error((const char *[]){_strerror(errno), NULL});
		return (1);
	}
	else 
		return (0);
}
