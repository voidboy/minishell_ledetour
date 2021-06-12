#include "minishell.h"

void	btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (root)
	{
		btree_apply_infix(root->left, applyf);
		applyf(root);
		btree_apply_infix(root->right, applyf);
	}
}

void	btree_apply_suffix(t_btree *root, void (*applyf)(void *))
{
	if (root)
	{
		btree_apply_suffix(root->left, applyf);
		btree_apply_suffix(root->right, applyf);
		applyf(root);
	}
}

int	max(int i, int j)
{
	if (i > j)
		return (i);
	return (j);
}

int	btree_level_count(t_btree *root)
{
	if (root)
		return (1 + max(btree_level_count(root->left), \
				btree_level_count(root->right)));
	return (0);
}

t_btree	*rightest_node(t_btree *root)
{
	if (root)
		if (root->right)
			return (rightest_node(root->right));
	return (root);
}
