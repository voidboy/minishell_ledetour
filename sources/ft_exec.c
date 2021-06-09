#include "minishell.h"

static void	ft_cleanup(t_btree *node)
{
	if (node->fd[0] != STDIN_FILENO)
		close(node->fd[0]);
	if (node->fd[1] != STDOUT_FILENO)
		close(node->fd[1]);
	/* cmd needs to be free */
	/* rdr needs to be free */
	/* arv needs to be free */
	/* tri needs to be free */
}

static int	launch_cmd(char *full_path, t_btree *node, t_dico *dico)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	exit_code = 0;
	if (pid == 0)
	{
		dup2(node->fd[0], STDIN_FILENO);
		dup2(node->fd[1], STDOUT_FILENO);
		ft_cleanup(node);
		printf("EXECV is %s\n", full_path);
		if (execve(full_path, node->argv, dico->envp) == -1)
		{
			exit_code = 126;
			ft_error(errno, NULL);
		}
	}
	else if (pid == -1)
	{
		exit_code = 1;
		ft_error(errno, NULL);
	}
	free(full_path);
	if (!exit_code)
	{
		wait(&exit_code);
		return (WEXITSTATUS(exit_code));
	}
	return exit_code;
}

static char	ft_exec_cmd(t_btree *node, t_dico *dico)
{
	char	*full_path;
	int		exit_code;

	(void)node;
	(void)dico;
	full_path = ft_search_path(node->argv[0], ft_get_dico_value("PATH", dico));
	if (full_path)
		exit_code = launch_cmd(full_path, node, dico);
	else
		exit_code = 127;
	ft_cleanup(node);
	return (exit_code);
}

static int	ft_exec_builtin(t_btree *node, t_dico *dico)
{
	int		i;
	char	**names;
	void	**fcts;
	int		(*ptr_fct)(t_btree *, t_dico *);

	names = (char *[]){"echo", "cd", "pwd", \
		"export", "unset", "env", "exit"};
	fcts = (void *[]){ft_echo, ft_cd, ft_pwd, \
		ft_export, ft_unset, ft_env, ft_exit};
	i = -1;
	while (++i < NBFCT && ft_strcmp(node->cmd, names[i]))
		;
	if (i < NBFCT)
	{
		ptr_fct = fcts[i];
		return ptr_fct(node, dico);
	}
	return -1;
}

int	ft_exec(t_btree *node, t_dico *dico)
{
	int	code_return;

	if (node->cmd == NULL)
		return (0);
	code_return = ft_exec_builtin(node, dico);
	if (code_return < 0)
		code_return = ft_exec_cmd(node, dico);
	printf("CMD:{%s}R:%d\n", node->cmd, code_return);
	return (code_return);
}
