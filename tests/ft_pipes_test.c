#include "minishell.h"

void test_pipes(t_btree *root)
{
	pid_t pid;

	ft_pipes(root->left);
	ft_pipes(root->right);
	if ((pid = fork()) == 0)
	{
		dup2(root->left->fd[1], 1);
		execve("/bin/echo", (char *[]){"echo","ddd", NULL}, NULL);
	}
	if ((pid = fork()) == 0)
	{
		dup2(root->right->fd[0], 0);
		execve("/bin/cat", (char *[]){"cat", NULL}, NULL);
	}
	sleep(1);
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	t_btree *root;

	root = ft_sow(ft_strjoin("ls | cat", "\n"));
	btree_show(root);
	test_pipes(root);
	btree_free(root);
}
