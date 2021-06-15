#include "minishell.h"

extern t_minishell	g_minishell;

int	ft_here_doc(t_btree *node, int start, int len)
{
	node->delimiter = ft_substr(node->cmd, start, len);
	if (!node->delimiter)
		ft_error((const char *[]){_strerror(errno), "\n", NULL}, TRUE);
	node->delimiter = ft_sanitize(node->delimiter);
	return (0);
}

int	ft_readoneline(t_btree *node)
{
	char		*tmp;
	char		*line;
	struct stat	s;

	line = readline("> ");
	if (!line || !ft_strcmp(line, node->delimiter))
	{
		if (!line && (fstat(STDIN_FILENO, &s) == -1))
		{
			free(node->buff);
			node->buff = NULL;
		}
		free(line);
		return (0);
	}
	line = add_linefeed(line);
	tmp = node->buff;
	node->buff = ft_strjoin(node->buff, line);
	free(tmp);
	free(line);
	return (1);
}

void	ft_here_doc_read(void *_node)
{
	t_btree	*node;
	int		i;

	node = _node;
	if (!node->delimiter)
		return ;
	echo_control_seq(FALSE);
	sig_apply(HERE_OPEN);
	i = dup(STDIN_FILENO);
	while (ft_readoneline(node))
		;
	dup2(i, STDIN_FILENO);
	sig_apply(PARENT);
}

int	ft_here_doc_open(t_btree *root)
{
	btree_apply_infix(root, ft_here_doc_read);
	return (0);
}
