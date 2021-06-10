#include "minishell.h"

void ft_here_doc_read(void *n)
{
	char	*tmp;
	char	*line;
	char 	*dline;
	t_btree *node;

	node = (t_btree *)n;
	if (!node->delimiter)
		return ;
	line = NULL;
	while (1)
	{
		write(STDIN_FILENO, "> ", 2);
		get_next_line(STDIN_FILENO, &line);
		//printf("line is [%s] : \n", line);
		dline = ft_strdup(line);
		dline[ft_strlen(line)-1] = 0;
		//printf("dline is {%s}\n", dline);
		if (!ft_strcmp(dline, node->delimiter))
		{
			free(dline);
			free(line);
			break ;
		}
		free(dline);
		tmp = node->buff;
		node->buff = ft_strjoin(node->buff, line);
		free(tmp);
		free(line);
	}
	//printf("node->buff is {%s}\n", node->buff);
}

int ft_open_her_doc(t_btree *root)
{
	btree_apply_infix(root, ft_here_doc_read);
	return 0;
}
