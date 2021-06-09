#include "minishell.h"

int main(int ac, char **argv)
{
	(void)ac;

	t_btree *root;
	argv++;
	while (*argv)
	{
		printf("\nline : %s\n\n", *argv);
		root = ft_sow(ft_strdup(*argv));
		btree_show(root);
		btree_free(root);
		argv++;
	}
}
