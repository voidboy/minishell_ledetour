#include "minishell.h"

extern t_minishell	g_minishell;

void	sig_apply(t_stage stage)
{
	void	(*ptr_hendler)(int n);
	void	**fcts;

	fcts = (void *[]){&sig_hand_parent, &sig_hand_here, &sig_hand_child};
	ptr_hendler = fcts[stage];
	signal(SIGINT, ptr_hendler);
	signal(SIGQUIT, ptr_hendler);
}

void	sig_hand_child(int n)
{
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	if (n == SIGQUIT)
	{
		rl_on_new_line();
	}
}

void	sig_hand_here(int n)
{
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
	if (n == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_hand_parent(int n)
{
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ft_set_dico_value(ft_strdup("?"), ft_strdup("1"),
			LOCAL, g_minishell.dico);
	}
	if (n == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}
