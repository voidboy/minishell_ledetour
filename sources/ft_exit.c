#include "minishell.h"

extern t_minishell	g_minishell;

static int	too_many_arguments(void)
{
	ft_error((const char *[]){_strerror(EEXIT),
		"too many arguments\n", NULL}, FALSE);
	return (1);
}

static void	numeric_argument_required(char *arg)
{
	ft_error((const char *[]){_strerror(EEXIT), arg,
		": numeric argument required\n", NULL}, FALSE);
	exit(EXIT_ERR);
}

int	ft_exit(t_btree *node, t_dico *dico)
{
	int	i;

	if (node->argv[1] && node->argv[2])
		return (too_many_arguments());
	else if (node->fd[0] == STDIN_FILENO && node->fd[1] == STDOUT_FILENO)
	{
		i = write(node->fd[1], "exit\n", 5) - 5;
		if (!node->argv[1])
		{
			ft_free(g_minishell.root, dico);
			exit(0);
		}
		while ((!i && (node->argv[1][i] == '-' || node->argv[1][i] == '+'))
			|| ft_isdigit(node->argv[1][i]))
			i++;
		if (node->argv[1][i] == '\0')
		{
			i = ft_atoi(node->argv[1]);
			ft_free(g_minishell.root, dico);
			exit(i);
		}
		else
			numeric_argument_required(node->argv[1]);
	}
	return (0);
}
