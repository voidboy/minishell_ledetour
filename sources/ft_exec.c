#include "minishell.h"

void	ft_cleanup_fd(t_btree *node)
{
	//printf("clean {%d}-{%d}\n", node->fd[0], node->fd[1]);
	if (node->fd[0] != -1 && node->fd[0] != STDIN_FILENO)
	{
		close(node->fd[0]);
		node->fd[0] = -1;
	}
	if (node->fd[1] != -1 && node->fd[1] != STDOUT_FILENO)
	{
		close(node->fd[1]);
		node->fd[1] = -1;
	}
	/* cmd needs to be free */
	/* rdr needs to be free */
	/* arv needs to be free */
	/* tri needs to be free */
}

static int	launch_cmd(char *full_path, t_btree *node, t_dico *dico)
{
	pid_t	pid;
	int		exit_code;
	struct termios conf;

	pid = fork();
	exit_code = 0;
	if (pid == 0)
	{
		/* Restore controls caracters */
		ioctl(ttyslot(), TIOCGETA, &conf);
		conf.c_lflag |= ECHOCTL;
		ioctl(ttyslot(), TIOCSETA, &conf);

		dup2(node->fd[0], STDIN_FILENO);
		dup2(node->fd[1], STDOUT_FILENO);
		ft_cleanup_fd(node);
		//printf("EXECV is {%s}\n", full_path);
		if (execve(full_path, node->argv, dico->envp) == -1)
		{
			ft_error((const char *[]){_strerror(EEMPTY), full_path, ": ", _strerror(errno), "\n", NULL}, FALSE);
			exit(CMD_FOUND_NX);
		}
	}
	else if (pid == -1)
	{
		free(full_path);
		exit_code = 1;
		ft_error((const char *[]){_strerror(errno),"\n", NULL}, TRUE);
	}
	if (!exit_code)
	{
		wait(&exit_code);
		if (WIFSIGNALED(exit_code))
		{
			if (WTERMSIG(exit_code) == SIGQUIT)
				write(STDERR_FILENO, "Quit: 3\n", 8);
			exit_code = SIG_TERM_NUM + WTERMSIG(exit_code);
		}
		else 
			exit_code = WEXITSTATUS(exit_code);
	}
	free(full_path);
	/* where node is clean ? */
	ft_cleanup_fd(node);
	return (exit_code);
}

static char	ft_exec_cmd(t_btree *node, t_dico *dico)
{
	char	*full_path;
	int		exit_code;
	char	*path;

	path = ft_get_dico_value("PATH", dico);
	full_path = ft_search_path(node->argv[0], path);
	free(path);
	if (full_path)
		exit_code = launch_cmd(full_path, node, dico);
	else
		exit_code = CMD_NFOUND;
	ft_cleanup_fd(node);
	return (exit_code);
}

static int	ft_exec_builtin(t_btree *node, t_dico *dico)
{
	int		code_return;
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
		code_return = ptr_fct(node, dico);
		ft_cleanup_fd(node);
		return (code_return);
	}
	return -1;
}

int	ft_exec(t_btree *node, t_dico *dico)
{
	int	code_return;

	if (node->cmd == NULL)
	{
		ft_cleanup_fd(node);
		return (0);
	}
	if (!*node->cmd)
	{
		ft_cleanup_fd(node);
		ft_error((const char *[]){_strerror(EEMPTY), "", ": command not found\n", NULL}, FALSE);
		return (CMD_NFOUND);
	}
	code_return = ft_exec_builtin(node, dico);
	if (code_return < 0)
		code_return = ft_exec_cmd(node, dico);
	return (code_return);
}
