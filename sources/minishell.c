#include "minishell.h"

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

	dico.sets = NULL;
	dico.envp = NULL;
	root = NULL;
	ft_set_dico(&dico, envp);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	r = 0;

	while (1)
	{

		line = readline("minishell> ");
		//printf("\nline : %s\n\n", line);
		if (!ft_strlen(line))
		{
			/* we should free here */
			write(STDOUT_FILENO, "exit\n", 5);
			//exit(r);
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
