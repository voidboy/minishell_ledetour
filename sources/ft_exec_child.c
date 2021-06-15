#include "minishell.h"

void	setup_child(char *full_path, t_btree *node, t_dico *dico)
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

void	lookup_child(int *exit_code)
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
