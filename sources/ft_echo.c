#include "minishell.h"

static t_bool	line_feed(t_btree *node, int *i)
{
	int		j;
	t_bool	lf;

	*i = 1;
	while (!ft_strncmp(node->argv[*i], "-n", 2))
	{
		lf = FALSE;
		j = 1;
		while (node->argv[*i][j] == 'n')
			j++;
		if (!node->argv[*i][j])
			(*i)++;
		else
		{
			lf = TRUE;
			break ;
		}
	}
	return (lf);
}

int	ft_echo(t_btree *node, t_dico *dico)
{
	int		i;
	t_bool	lf;

	(void)dico;
	lf = line_feed(node, &i);
	while (node->argv[i])
	{
		write(node->fd[1], node->argv[i], ft_strlen(node->argv[i]));
		if (node->argv[i++ + 1])
			write(node->fd[1], " ", 1);
	}
	if (lf)
		write(node->fd[1], "\n", 1);
	return (0);
}
