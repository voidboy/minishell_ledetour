#include "minishell.h"

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	t_btree *root;
	t_dico	*dico;

	dico = NULL;
	root = ft_sow(ft_strjoin(*++av, "\n"));
	btree_show(root);
	root->cmd = ft_expander(root->cmd, dico);
	//btree_free(root);
}
