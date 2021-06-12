#include "minishell.h"

void	ft_pipes(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
		{
			if (pipe(node->parent->fd) == -1)
				ft_error((const char *[]){_strerror(errno), "\n"}, TRUE);
			else
				node->fd[1] = node->parent->fd[1];
		}
		else if (node->side == RIGHT)
		{
			node->fd[0] = node->parent->fd[0];
			if (node->parent->parent && node->parent->parent->type == PIPE)
			{
				if (pipe(node->parent->parent->fd) == -1)
					ft_error((const char *[]){_strerror(errno), "\n"}, TRUE);
				else
					node->fd[1] = node->parent->parent->fd[1];
			}
		}
	}
}
