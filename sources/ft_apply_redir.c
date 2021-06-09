#include "minishell.h"

static int update_fds(t_btree *node, t_way r, char *f)
{
	int	fd;
	int opts;

	if (r == OUT)
		opts = O_WRONLY | O_CREAT | O_TRUNC;
	else if (r == OUT_OUT)
		opts = O_WRONLY | O_APPEND | O_CREAT;
	else 
		opts = O_RDONLY;
	/* default permission ? */
	fd = open(f, opts, 0644);
	if (fd == -1)
		return (ft_error(errno, NULL));
	if (r == OUT_OUT || r == OUT)
	{
		if (node->fd[1] != STDOUT_FILENO)
			close(node->fd[1]);
		node->fd[1] = fd;
	}
	else 
	{
		if (node->fd[0] != STDIN_FILENO)
			close(node->fd[0]);
		node->fd[0] = fd;
	}
	return (SUCCESS);
}

int ft_apply_redir(t_btree *node, t_dico *dico)
{
	t_list	*current;
	t_redir *redir;
	char	*filename;

	current = node->redir;
	while (current)
	{
		redir = current->content;
		if (redir->filename[0] == '$')
		{
			//printf("1> filename is {%s}\n", redir->filename);
			/* protect this, need to free filename */
			filename = strdup(redir->filename);
			redir->filename = ft_expander(redir->filename, dico);
			//printf("2> filename is {%s}\n", redir->filename);
			if (!*redir->filename)
				return (ft_error(EEMPTY, (const char *[]){filename, ": ambiguous redirect\n", NULL}));
		}
		else 
			redir->filename = ft_expander(redir->filename, dico);
		update_fds(node, redir->way, redir->filename);
		current = current->next;
	}
	return (SUCCESS);
}
