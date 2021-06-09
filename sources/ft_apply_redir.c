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
	{
		ft_error(errno, NULL);
		return (1);
	}
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
	int		exit_code;

	exit_code = 0;
	current = node->redir;
	while (current && !exit_code)
	{
		redir = current->content;
		redir->filename = ft_sanitize(ft_expander(redir->filename, dico));
		exit_code = update_fds(node, redir->way, redir->filename);
		current = current->next;
	}
	return (exit_code);
}
