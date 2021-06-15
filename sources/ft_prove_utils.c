#include "minishell.h"

int	ft_here_doc(t_btree *node, int start, int len)
{
	node->delimiter = ft_substr(node->cmd, start, len);
	if (!node->delimiter)
		ft_error((const char *[]){_strerror(errno), "\n", NULL}, TRUE);
	node->delimiter = ft_sanitize(node->delimiter);
	return (0);
}
