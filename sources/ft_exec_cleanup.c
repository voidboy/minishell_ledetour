#include "minishell.h"

void	ft_cleanup_parent(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
		{
			if (node->parent->fd[1] != STDOUT_FILENO)
				close(node->parent->fd[1]);
		}
		else if (node->side == RIGHT)
		{
			if (node->parent->fd[0] != STDIN_FILENO)
				close(node->parent->fd[0]);
			if (node->parent->parent && node->parent->parent->type == PIPE)
			{
				if (node->parent->parent->fd[1] != STDOUT_FILENO)
					close(node->parent->parent->fd[1]);
			}
		}
	}
}

void	ft_cleanup_child(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
		{
			if (node->parent->fd[0] != STDIN_FILENO)
				close(node->parent->fd[0]);
		}
		else if (node->side == RIGHT)
		{
			if (node->parent->fd[1] != STDOUT_FILENO)
				close(node->parent->fd[1]);
			if (node->parent->parent && node->parent->parent->type == PIPE)
			{
				if (node->parent->parent->fd[0] != STDIN_FILENO)
					close(node->parent->parent->fd[0]);
			}
		}
	}
}
