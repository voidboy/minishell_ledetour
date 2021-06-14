#include "minishell.h"

void	ft_set_vars(t_btree *node, t_dico *dico, int code_return)
{
	char	**argv;
	int		i;

	ft_set_dico_value(ft_strdup("?"), ft_itoa(code_return), LOCAL, dico);
	argv = node->argv;
	i = -1;
	while (argv && argv[++i])
		;
	if (argv && i > 0)
		ft_set_dico_value(ft_strdup("_"), ft_strdup(argv[i - 1]), GLOBAL, dico);
}

int	ft_execute_node(t_btree *node, t_dico *dico)
{
	ft_pipes(node);
	ft_redir(node);
	ft_assign(node, dico);
	if (ft_apply_redir(node, dico) == 0)
		return (ft_exec(node, dico));
	return (1);
}

int	ft_cross(t_btree *root, t_dico *dico)
{
	static int	code_return;
	char		*str_return;

	str_return = ft_get_dico_value("?", dico);
	code_return = ft_atoi(str_return);
	free(str_return);
	if (code_return != EXIT_FORK && root)
	{
		code_return = ft_cross(root->left, dico);
		ft_set_vars(root, dico, code_return);
		if (code_return != EXIT_FORK && root->type == CMD)
		{
			code_return = ft_execute_node(root, dico);
			ft_set_vars(root, dico, code_return);
		}
		else if (code_return != EXIT_FORK && !((root->type == OR && code_return == 0) \
						|| (root->type == AND && code_return != 0 )))
		{
			code_return = ft_cross(root->right, dico);
			ft_set_vars(root, dico, code_return);
		}
	}
	return (code_return);
}
