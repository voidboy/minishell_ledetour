#include "minishell.h"

extern t_minishell g_minishell;

void	ft_here_doc_read(void *_node)
{
	char	*tmp;
	char	*line;
	t_btree	*node;
	struct stat s;
	int i;

	node = _node;
	if (!node->delimiter)
		return ;
	line = NULL;
	echo_control_seq(FALSE);
	sig_apply(HERE_OPEN);
	i = dup(STDIN_FILENO);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, node->delimiter))
		{
			if (!line && (fstat(STDIN_FILENO, &s) == -1))
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
	sig_apply(PARENT);
}

int	ft_here_doc_open(t_btree *root)
{
	btree_apply_infix(root, ft_here_doc_read);
	return (0);
}
