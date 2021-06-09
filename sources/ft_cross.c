#include "minishell.h"

int ft_execute_node(t_btree *node, t_dico *dico)
{
	ft_pipes(node);
	ft_redir(node);
	ft_assign(node, dico);
	ft_apply_redir(node, dico);
	return (ft_exec(node, dico));
}

int ft_cross(t_btree *root, t_dico *dico)
{
	static int code_return;

	if (root)
	{
		code_return = ft_cross(root->left, dico);
		if (root->type == CMD)
		{
			code_return = ft_execute_node(root, dico);
			printf("r:%d\n", code_return);
		}
		else 
			if (!((root->type == OR && code_return == 0) \
						|| (root->type == AND && code_return != 0 )))
			code_return = ft_cross(root->right, dico);
	}
	return code_return;
}
