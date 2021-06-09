#include "minishell.h"

int ft_echo(t_btree *node, t_dico *dico)
{
	int		i;
	int		j;
	t_bool	nolf;

	(void)dico;
	nolf = FALSE;
	i = 1;
	while (!ft_strncmp(node->argv[i], "-n", 2))
	{
		j = 1;
		while (node->argv[i][j] == 'n')
			j++;
		if (node->argv[i][j])
			break ; 
		nolf = TRUE;
		i++;
	}
	while (node->argv[i])
	{
		write(STDOUT_FILENO, node->argv[i], ft_strlen(node->argv[i]));
		if (node->argv[i++ + 1])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!nolf)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int ft_cd(t_btree *node, t_dico *dico)
{
	(void)dico;
	if (chdir(node->argv[1]) == -1)
	{
		ft_error(errno, NULL);
		return (1);
	}
	else 
		return (0);
}


int ft_pwd(t_btree *node, t_dico *dico)
{
	char cwd[__DARWIN_MAXNAMLEN];

	(void)dico;
	if (getcwd(cwd, __DARWIN_MAXNAMLEN) == NULL)
	{
		ft_error(errno, NULL);
		return (1);
	}
	else 
	{
		write(node->fd[1], cwd, ft_strlen(cwd));
		return (0);
	}
}

int ft_export(t_btree *node, t_dico *dico)
{
	(void)node;
	(void)dico;
	printf("#### export\n");
	return (0);
}

int ft_unset(t_btree *node, t_dico *dico)
{
	(void)node;
	(void)dico;
	printf("#### unset\n");
	return (0);
}

int ft_env(t_btree *node, t_dico *dico)
{
	(void)node;
	(void)dico;
	printf("#### env\n");
	return (0);
}

int ft_exit(t_btree *node, t_dico dico)
{
	int i;

	(void)dico;
	if (node->argv[1] && node->argv[2])
	{
		ft_error(EEMPTY, (const char *[]){"exit: too many arguments\n", NULL});
		return (1);
	}
	else if (node->fd[0] == STDIN_FILENO && node->fd[1] == STDOUT_FILENO)
	{
		i = 0;
		write(STDOUT_FILENO, "exit\n", 5);
		while (ft_isdigit(node->argv[1][i]))
			i++;
		if (node->argv[1][i] == '\0')
			exit(ft_atoi(node->argv[1]));
		else 
			ft_error(EEMPTY, (const char *[]){"exit: ", node->argv[1],
				": numeric argument required\n", NULL});
	}
	return (0);
}
