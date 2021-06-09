#include "minishell.h"

static void enable_raw(void)
{
	struct termios term_conf;

	tcgetattr(STDIN_FILENO, &term_conf);
	term_conf.c_lflag &= ~(ICANON | ECHOCTL | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term_conf);
}

static void disable_raw(void)
{
	struct termios term_conf;

	tcgetattr(STDIN_FILENO, &term_conf);
	term_conf.c_lflag |= (ICANON | ECHOCTL | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term_conf);
}

void sigint_handler(int n)
{
	if (n == SIGINT)
		write(STDOUT_FILENO, "\nminishell> ", 12);
	else 
		;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char			*up_key;
	char			*dw_key;
	t_dico			dico;
	char			*line;
	char			*last;
	t_btree			*root;
	int				i;
	int				esc;
   
	tgetent(NULL, getenv("TERM"));
	up_key = tgetstr("ku", NULL);
	dw_key = tgetstr("kd", NULL);
	dico.sets = NULL;
	dico.envp = NULL;
	ft_set_dico(&dico, envp);
	setvbuf(stdout, NULL, _IONBF, 0);
	enable_raw();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_DFL);
	last = NULL;
	while (1)
	{
		line = malloc(sizeof(char) * 4096);
		i = 0;
		write(STDOUT_FILENO, "minishell> ", 11);
		esc  = 0;
		while (1)
		{
			while (!read(STDIN_FILENO, &line[i], 1))
				;
			/* start escape seq. mode */
			if (line[i] == '\x1b')
				esc = i;
			/* UP */
			if (!ft_strncmp(line + esc, "\x1b\x5b\x42", 3))
			{
				line = NULL;
				write(STDOUT_FILENO, "minishell> ", 11);
				if (last) 
					write(STDOUT_FILENO, last, ft_strlen(last));
				break ;
			}
			/* DOWN */
			else if (!ft_strncmp(line + esc, "\x1b\x5b\x41", 3))
			{
				line = NULL;
				write(STDOUT_FILENO, "minishell> ", 11);
				if (last) 
					write(STDOUT_FILENO, last, ft_strlen(last));
				break ;
			}
			else if (line[i] == '\n')
			{
				line[++i] = '\0';
				last = strdup(line);
				break ;
			}
			else 
				write(STDOUT_FILENO, &line[i++], 1);
		}
		//get_next_line(STDIN_FILENO, &line);
		//printf("\nline : %s\n\n", line);
		root = ft_sow(line);
		btree_show(root);
		if (ft_prove(root) != -1)
		{
			ft_cross(root, &dico);
			btree_show(root);
			ft_lstiter(dico.sets, ft_show_dico);
		}
		btree_free(root);
	}
	disable_raw();
}
