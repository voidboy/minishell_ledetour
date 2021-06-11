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
		ft_error((const char *[]){_strerror(EEMPTY), f, ": ", _strerror(errno), "\n", NULL}, FALSE);
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

int ft_apply_here_doc(t_btree *node, t_dico *dico)
{
	int fd[2];

	pipe(fd);

	node->buff = ft_expander(node->buff, dico);
	write(fd[1], node->buff, ft_strlen(node->buff));
	node->fd[0] = fd[0];
	close(fd[1]);
	return (0);
}

int ft_apply_redir(t_btree *node, t_dico *dico)
{
	t_list	*current;
	t_redir *redir;
	char	*filename;
	int		exit_code;

	current = node->redir;
	exit_code = 0;
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
			{
				ft_error((const char *[]){_strerror(EEMPTY), filename, ": ambiguous redirect\n", NULL}, FALSE);
				free(filename);
				return (1);
			}
			redir->filename = ft_sanitize(redir->filename);
		}
		else 
			redir->filename = ft_expander(redir->filename, dico);
		if (redir->way == IN_IN)
			exit_code = ft_apply_here_doc(node, dico);
		else 
			exit_code = update_fds(node, redir->way, redir->filename);
		if (exit_code)
			return (exit_code);
		current = current->next;
	}
	return (exit_code);
}
