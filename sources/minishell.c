#include "minishell.h"

t_minishell g_minishell;

static void sig_handler(int n)
{
	struct termios conf;

	(void)n;
	if (isatty(STDIN_FILENO))
	{
		ioctl(ttyslot(), TIOCGETA, &conf);
		conf.c_lflag &= ~(ECHOCTL);
		ioctl(ttyslot(), TIOCSETA, &conf);
	}
	if (n == SIGINT)
		write(STDOUT_FILENO, "\nminishell> ", 12);
}

int main(int ac, char **argv, char **envp)
{
	(void)ac;
	char	*line;
	t_btree	*root;
	t_dico	dico;
   	int		prove;
	struct termios conf;

	(void)argv;
	dico.sets = NULL;
	dico.envp = NULL;
	root = NULL;
	ft_set_dico(&dico, envp);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while (1)
	{
		line = readline("minishell> ");
		rl_on_new_line();
		//printf("\nline : %s\n\n", line);
		if (!ft_strlen(line))
		{
			/* we should free here */
			write(STDOUT_FILENO, "exit\n", 5);
			exit(0);
		}
		root = ft_sow(line);
		//btree_show(root);
		prove = ft_prove(root);
		ft_open_her_doc(root);
		if ( prove != -1 )
		{
			ft_cross(root, &dico);
			//btree_show(root);
			//ft_lstiter(dico.sets, ft_show_dico);
		}
		btree_apply_suffix(root, ft_free_node);
		root = NULL;
	}
}
