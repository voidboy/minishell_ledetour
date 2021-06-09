#include "minishell.h"

void ft_pipes(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
			node->fd[1] = node->parent->fd[1];
		else if (node->side == RIGHT)
		{
			node->fd[0] = node->parent->fd[0];
			if (node->parent->parent && node->parent->parent->type == PIPE)
				node->fd[1] = node->parent->parent->fd[1];
		}
	}
}
