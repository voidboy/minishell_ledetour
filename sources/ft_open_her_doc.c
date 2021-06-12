#include "minishell.h"

void	ft_here_doc_read(void *n)
{
	char	*tmp;
	char	*line;
	t_btree	*node;

	node = (t_btree *)n;
	if (!node->delimiter)
		return ;
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, node->delimiter))
		{
			free(line);
			break ;
		}
		tmp = node->buff;
		node->buff = ft_strjoin(node->buff, line);
		free(tmp);
		free(line);
	}
}

int	ft_open_her_doc(t_btree *root)
{
	btree_apply_infix(root, ft_here_doc_read);
	return (0);
}
