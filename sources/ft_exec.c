#include "minishell.h"

void	ft_cleanup_parent(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
		{
			if (node->parent->fd[1] != STDOUT_FILENO)
				close(node->parent->fd[1]);
		}
		else if (node->side == RIGHT)
		{
			if (node->parent->fd[0] != STDIN_FILENO)
				close(node->parent->fd[0]);
			if (node->parent->parent && node->parent->parent->type == PIPE)
			{
				if (node->parent->parent->fd[1] != STDOUT_FILENO)
					close(node->parent->parent->fd[1]);
			}
		}
	}
}

void	ft_cleanup_child(t_btree *node)
{
	if (node->parent && node->parent->type == PIPE)
	{
		if (node->side == LEFT)
		{
			if (node->parent->fd[0] != STDIN_FILENO)
				close(node->parent->fd[0]);
		}
		else if (node->side == RIGHT)
		{
			if (node->parent->fd[1] != STDOUT_FILENO)
				close(node->parent->fd[1]);
			if (node->parent->parent && node->parent->parent->type == PIPE)
			{
				if (node->parent->parent->fd[0] != STDIN_FILENO)
					close(node->parent->parent->fd[0]);
			}
		}
	}
}

static void	setup_child(char *full_path, t_btree *node, t_dico *dico)
{
	echo_control_seq(TRUE);
	dup2(node->fd[0], STDIN_FILENO);
	dup2(node->fd[1], STDOUT_FILENO);
	ft_cleanup_child(node);
	if (execve(full_path, node->argv, dico->envp) == -1)
	{
		ft_error((const char *[]){_strerror(EEMPTY), full_path,
			": ", _strerror(errno), "\n", NULL}, FALSE);
		exit(CMD_FOUND_NX);
	}
}

static void	lookup_child(int *exit_code)
{
	if (WIFSIGNALED(*exit_code))
	{
		if (WTERMSIG(*exit_code) == SIGQUIT)
			write(STDERR_FILENO, "Quit: 3\n", 8);
		*exit_code = SIG_TERM_NUM + WTERMSIG(*exit_code);
	}
	else
		*exit_code = WEXITSTATUS(*exit_code);
}

static int	launch_cmd(char *full_path, t_btree *node, t_dico *dico)
{
	pid_t			pid;
	int				exit_code;

	exit_code = 0;
	pid = fork();
	if (pid == 0)
		setup_child(full_path, node, dico);
	else if (pid == -1)
	{
		free(full_path);
		ft_error((const char *[]){_strerror(EEMPTY), \
				"fork: ", _strerror(errno), "\n", NULL}, FALSE);
		ft_cleanup_parent(node);
		return (EXIT_FORK);
	}
	ft_cleanup_parent(node);
	if (!exit_code && node->fd[1] == STDOUT_FILENO)
	{
		sig_apply(CHILD);
		wait(&exit_code);
		sig_apply(PARENT);
		lookup_child(&exit_code);
	}
	free(full_path);
	return (exit_code);
}

static int	ft_exec_cmd(t_btree *node, t_dico *dico)
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
		ft_cleanup_parent(node);
		return (code_return);
	}
	return (-1);
}

int	ft_exec(t_btree *node, t_dico *dico)
{
	int	code_return;

	if (node->cmd == NULL)
	{
		ft_cleanup_parent(node);
		return (0);
	}
	if (!*node->cmd)
	{
		ft_cleanup_parent(node);
		ft_error((const char *[]){_strerror(EEMPTY), \
				": command not found\n", NULL}, FALSE);
		return (CMD_NFOUND);
	}
	code_return = ft_exec_builtin(node, dico);
	if (code_return < 0)
		code_return = ft_exec_cmd(node, dico);
	return (code_return);
}
