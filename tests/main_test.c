#include "minishell.h"

int main(int ac, char **argv, char **envp)
{
	(void)ac;
	t_btree *root;
	t_dico dico;
   
//	argv = (char *[]){"",
//			"var=1;' ' > $var",
	//	" '    '1234\\ \" \"5\\ | hh \"&&\" || ds",
	//	"var=ls;\"/bin''/\"$var",
//		NULL};
	dico.sets = NULL;
	dico.envp = NULL;
	ft_set_dico(&dico, envp);
	argv++;
	while (*argv)
	{
		printf("\nline : %s\n\n", *argv);
		root = ft_sow(ft_strjoin(*argv, "\n"));
		//btree_show(root);
		if (1 || ft_prove(root) != -1)
		{
			ft_cross(root, &dico);
		//	btree_show(root);
		//	ft_lstiter(dico.sets, ft_show_dico);
		}
		btree_free(root);
		argv++;
	}
}
