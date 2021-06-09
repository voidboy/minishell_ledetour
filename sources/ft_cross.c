#include "minishell.h"

void ft_set_vars(t_var *vars, t_dico *dico, int nbvars)
{
	int i;

	i= -1;
	while (++i < nbvars)
	{
		ft_set_dico_value(vars[i].key, vars[i].value, vars[i].scope, dico);
	}
}

int ft_execute_node(t_btree *node, t_dico *dico)
{
	ft_pipes(node);
	ft_redir(node);
	ft_assign(node, dico);
	if (ft_apply_redir(node, dico) == 0)
		return (ft_exec(node, dico));
	return 1;
}

int ft_cross(t_btree *root, t_dico *dico)
{
	static int code_return;

	if (root)
	{
		code_return = ft_cross(root->left, dico);
		ft_set_dico_value(ft_strdup("?"), ft_itoa(code_return), LOCAL, dico);
		if (root->type == CMD)
		{
			code_return = ft_execute_node(root, dico);
			ft_set_dico_value(ft_strdup("?"), ft_itoa(code_return), LOCAL, dico);
		}
		else if (!((root->type == OR && code_return == 0) \
						|| (root->type == AND && code_return != 0 )))
		{
			code_return = ft_cross(root->right, dico);
			ft_set_dico_value(ft_strdup("?"), ft_itoa(code_return), LOCAL, dico);
		}
	}
	return code_return;
}
