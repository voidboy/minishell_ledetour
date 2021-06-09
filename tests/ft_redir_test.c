#include "minishell.h"

void display_redirs(void *content)
{
	char	*filename;
	t_redir	*redir;
	t_way	way;
	t_list	*list;

	list = ((t_btree *)content)->redir;
	printf("REDIR(S) {%s} :\n", ((t_btree *)content)->cmd);
	while (list)
	{
		redir = list->content;
		filename = redir->filename;
		way = redir->way;
		printf("FILENAME {%s}\n", filename);
		printf("WAY {%s}\n", way == IN ? "in" :
				way == OUT ? "out": "out out");
		list = list->next;
	}
	printf("==============================\n");
}

int main(int ac, char **argv)
{
	(void)ac;

	t_btree *root;

	argv++;
	while (*argv)
	{
		printf("\nline : %s\n\n", *argv);
		root = ft_sow(ft_strjoin(*argv, "\n"));
		btree_show(root);
		btree_apply_infix(root, ft_redir);
		btree_apply_infix(root, display_redirs);
		btree_free(root);
		argv++;
	}
}
