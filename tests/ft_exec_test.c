#include "minishell.h"

void ft_show_dico(void *content)
{
	t_var *var;

	var = content;
	if (var->scope == LOCAL)
		printf("DICO [ key : %s value : %s scope : %s]\n", var->key, var->value, var->scope == LOCAL ? "LOCAL" : "GLOBAL");
}

int main(int ac, char **argv, char **envp)
{
	(void)ac;
	t_btree *root;
	t_dico dico;

	dico.sets = NULL;
	dico.envp = NULL;
	ft_set_dico(&dico, envp);
	argv++;
	while (*argv)
	{
		printf("\nline : %s\n\n", *argv);
		root = ft_sow(ft_strjoin(*argv, "\n"));
		btree_show(root);
		if (ft_prove(root) != -1)
		{
			ft_cross(root, &dico);
			btree_show(root);
			ft_lstiter(dico.sets, ft_show_dico);
		}
		btree_free(root);
		argv++;
	}
}
