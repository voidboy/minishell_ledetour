#include "minishell.h"

int main(void)
{
	t_btree *tree;

	tree = malloc(sizeof(t_btree));
	tree->parent = NULL;
	tree->left = NULL;
	tree->right = NULL;
	tree->cmd = "> \"''cmd2''\\\"  \" > cmd3 echo bonjour > '  \"cmd1';";
	tree->type = CMD;
	if (ft_prove(tree) == SUCCESS)
		printf("Tree is valid\n");
	else 
		printf("Tree is invalid\n");
	printf("CMD is {%s}\n", tree->cmd);
	return (0);
}

