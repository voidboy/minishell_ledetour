#include "minishell.h"

int ft_exit(t_btree *node, t_dico dico)
{
	int i;

	(void)dico;
	if (node->argv[1] && node->argv[2])
	{
		ft_error((const char *[]){_strerror(EEXIT),
			"too many arguments\n", NULL}, FALSE);
		return (1);
	}
	else if (node->fd[0] == STDIN_FILENO && node->fd[1] == STDOUT_FILENO)
	{
		i = 0;
		write(node->fd[1], "exit\n", 5);
		if (!node->argv[1])
		{
			ft_free(g_minishell.root, g_minishell.dico);
			exit(0);
		}
		while ((!i && (node->argv[1][i] == '-' || node->argv[1][i] == '+'))
			|| ft_isdigit(node->argv[1][i]))
			i++;
		if (node->argv[1][i] == '\0')
		{
			ft_free(g_minishell.root, g_minishell.dico);
			exit(ft_atoi(node->argv[1]));
		}
		else 
			ft_error((const char *[]){_strerror(EEXIT), node->argv[1],
				": numeric argument required\n", NULL}, FALSE);
	}
	return (0);
}
