#include "minishell.h"

void	ft_here_doc_read(void *_node)
{
	char	*tmp;
	char	*line;
	t_btree	*node;
	int i;

	node = _node;
	if (!node->delimiter)
		return ;
	line = NULL;
	echo_control_seq(FALSE);
	signal(SIGINT, sig_handler1);
	signal(SIGQUIT, sig_handler1);
	i = dup(STDIN_FILENO);
	while (1)
	{
		line = readline("> ");
		//printf("%s\n", line);
		if (!line || !ft_strcmp(line, node->delimiter))
		{
			if (!line)
			{
				free(node->buff);
				node->buff = NULL;
			}
			free(line);
			break ;
		}
		line = add_linefeed(line);
		tmp = node->buff;
		node->buff = ft_strjoin(node->buff, line);
		free(tmp);
		free(line);
	}
	dup2(i, STDIN_FILENO);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	echo_control_seq(TRUE);
}

int	ft_here_doc_open(t_btree *root)
{
	btree_apply_infix(root, ft_here_doc_read);
	return (0);
}
