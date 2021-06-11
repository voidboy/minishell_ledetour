#include "minishell.h"

void ft_pipes(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
		{
			pipe(node->parent->fd);
		//	printf("pipe F {%d}-{%d}\n", node->parent->fd[0], node->parent->fd[1]);
			node->fd[1] = node->parent->fd[1];
		}
		else if (node->side == RIGHT)
		{
			node->fd[0] = node->parent->fd[0];
			if (node->parent->parent && node->parent->parent->type == PIPE)
			{
				pipe(node->parent->parent->fd);
				//printf("pipe GF {%d}-{%d}\n", node->parent->parent->fd[0], node->parent->parent->fd[1]);
				node->fd[1] = node->parent->parent->fd[1];
			}
		}
	}
}
