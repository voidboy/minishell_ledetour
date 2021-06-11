#include "minishell.h"
#include "termios.h"
#include "sys/ioctl.h"

t_minishell g_minishell;

static void sigint_handler(int n)
{
	(void)n;
	write(STDOUT_FILENO, "\nminishell> ", 12);
}

static void sigquit_handler(int n)
{
	(void)n;
}

int main(int ac, char **argv, char **envp)
{
	(void)ac;
	char	*line;
	t_btree	*root;
	t_dico	dico;
   	int		prove;
	int		r;
	struct termios conf;

	argv = (char *[]){"",
			"var=1;' ' > $var",
	//	" '    '1234\\ \" \"5\\ | hh \"&&\" || ds",
	//	"var=ls;\"/bin''/\"$var",
		NULL};
	dico.sets = NULL;
	dico.envp = NULL;
	root = NULL;
	ft_set_dico(&dico, envp);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	r = 0;
	while (1)
	{
		/* Hide controls caracters */
		ioctl(ttyslot(), TIOCGETA, &conf);
		conf.c_lflag &= ~(ECHOCTL);
		ioctl(ttyslot(), TIOCSETA, &conf);

		if (!(WTERMSIG(r) == SIGINT))
			write(STDOUT_FILENO, "minishell> ", 11);
		get_next_line(STDIN_FILENO, &line);
		if (!ft_strlen(line))
		{
			/* we should free here */
			write(STDOUT_FILENO, "exit\n", 5);
			exit(r);
		}
		//printf("\nline : %s\n\n", line);
		root = ft_sow(line);
		//btree_show(root);
		prove = ft_prove(root);
		ft_open_her_doc(root);
		if ( prove != -1 )
		{
			r = ft_cross(root, &dico);
			//btree_show(root);
			//ft_lstiter(dico.sets, ft_show_dico);
		}
		btree_apply_suffix(root, ft_free_node);
		root = NULL;
	}
}
