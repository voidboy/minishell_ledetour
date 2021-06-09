#include "minishell.h"

int ft_env(t_btree *node, t_dico *dico)
{
	(void)node;
	ft_show_envp(dico, 0, node->fd[1]);
	return (0);
}
