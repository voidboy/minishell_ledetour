#include "minishell.h"

int	ft_env(t_btree *node, t_dico *dico)
{
	ft_set_dico_value(ft_strdup("_"), ft_strdup("env"), GLOBAL, dico);
	ft_show_envp(dico, 0, node->fd[1]);
	return (0);
}
