#include "minishell.h"

t_minishell g_minishell;

void sigint_handler(int n)
{
	if (n == SIGINT)
		write(STDOUT_FILENO, "\nminigshell> ", 12);
	else
		;
}

void sigquit_handler(int n)
{
	(void)n;
	;
}

int main(int ac, char **argv, char **envp)
{
	(void)ac;
	char	*line;
	t_btree	*root;
	t_dico	dico;
   	int prove;
	argv = (char *[]){"",
			"var=1;' ' > $var",
	//	" '    '1234\\ \" \"5\\ | hh \"&&\" || ds",
	//	"var=ls;\"/bin''/\"$var",
		NULL};
	dico.sets = NULL;
	dico.envp = NULL;
	root = NULL;
	ft_set_dico(&dico, envp);
	//signal(SIGINT, sigint_handler);
	//signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		write(STDOUT_FILENO, "minishell> ", 11);
		get_next_line(STDIN_FILENO, &line);
		//printf("\nline : %s\n\n", line);
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
