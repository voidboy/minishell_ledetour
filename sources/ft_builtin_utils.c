#include "minishell.h"

int ft_echo(t_btree *node, t_dico *dico)
{
	(void)node;
	(void)dico;
	printf("%s\n", node->argv[1]);
	return (0);
}

int ft_cd(t_btree *node, t_dico *dico)
{
	(void)node;
	(void)dico;
	printf("#### cd\n");
	return (0);
}


int ft_pwd(t_btree *node, t_dico *dico)
{
	(void)node;
	(void)dico;
	printf("#### pwd\n");
	return (0);
}

int ft_exit(t_btree *node, t_dico dico)
{
	(void)node;
	(void)dico;
	printf("#### exit\n");
	return (0);
}
