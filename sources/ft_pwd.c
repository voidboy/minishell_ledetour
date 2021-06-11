#include "minishell.h"

int ft_pwd(t_btree *node, t_dico *dico)
{
	char cwd[__DARWIN_MAXNAMLEN];

	(void)dico;
	if (getcwd(cwd, __DARWIN_MAXNAMLEN) == NULL)
	{
		ft_error((const char *[]){_strerror(errno), "\n",NULL}, FALSE);
		return (1);
	}
	else 
	{
		write(node->fd[1], cwd, ft_strlen(cwd));
		write(node->fd[1], "\n", 1);
		return (0);
	}
}
