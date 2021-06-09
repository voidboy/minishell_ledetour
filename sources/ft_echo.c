#include "minishell.h"

int ft_echo(t_btree *node, t_dico *dico)
{
	int		i;
	int		j;
	t_bool	nolf;

	(void)dico;
	nolf = FALSE;
	i = 1;
	while (!ft_strncmp(node->argv[i], "-n", 2))
	{
		j = 1;
		while (node->argv[i][j] == 'n')
			j++;
		if (node->argv[i][j])
			break ; 
		nolf = TRUE;
		i++;
	}
	while (node->argv[i])
	{
		write(node->fd[1], node->argv[i], ft_strlen(node->argv[i]));
		if (node->argv[i++ + 1])
			write(node->fd[1], " ", 1);
	}
	if (!nolf)
		write(node->fd[1], "\n", 1);
	return (0);
}
