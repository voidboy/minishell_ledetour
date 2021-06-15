#include "minishell.h"

int	ft_free_strs(char **strs)
{
	int	i;

	i = -1;
	while (strs && strs[++i])
		free(strs[i]);
	free(strs);
	return (0);
}

void	ft_free_redir(void *content)
{
	free(((t_redir *)content)->filename);
	free(content);
}

void	ft_free_node(void *_node)
{
	t_btree	*node;

	node = _node;
	ft_lstclear(&(node->redir), ft_free_redir);
	free(node->cmd);
	ft_free_strs(node->argv);
	free(node->buff);
	free(node->delimiter);
	free(node);
}

void	ft_free_sets(void *content)
{
	free(((t_var *)content)->key);
	free(((t_var *)content)->value);
	free(content);
}

void	ft_free(t_btree *root, t_dico *dico)
{
	btree_apply_suffix(root, ft_free_node);
	ft_lstclear(&(dico->sets), ft_free_sets);
	ft_free_strs(dico->envp);
}
