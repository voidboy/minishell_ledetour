#include "minishell.h"

t_minishell	g_minishell;

void	init_minishell(t_dico *dico, t_btree **root, char **envp)
{
	dico->sets = NULL;
	dico->envp = NULL;
	*root = NULL;
	echo_control_seq(FALSE);
	sig_apply(PARENT);
	ft_set_dico(dico, envp);
}

void	run_command(t_btree *root, t_dico *dico)
{
	if (ft_cross(root, dico) == EXIT_FORK)
		ft_set_dico_value(ft_strdup("?"), ft_strdup("1"), LOCAL, dico);
	sig_apply(CHILD);
	while (wait(NULL) != -1)
		;
	sig_apply(PARENT);
	echo_control_seq(FALSE);
}

char	*get_the_next_line(void)
{	
	char	*line;

	line = readline("minishell> ");
	if (!line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		exit(0);
	}
	if (ft_strlen(line))
		add_history(line);
	return (line);
}

int	main(int ac, char **argv, char **envp)
{
	char	*line;
	t_btree	*root;
	t_dico	dico;
	int		prove;

	(void)argv;
	(void)ac;
	init_minishell(&dico, &root, envp);
	while (1)
	{
		echo_control_seq(FALSE);
		line = get_the_next_line();
		root = ft_sow(line);
		prove = ft_prove(root);
		ft_here_doc_open(root);
		if (prove != -1)
			run_command(root, &dico);
		else
			ft_set_dico_value(ft_strdup("?"), ft_strdup("258"), LOCAL, &dico);
		btree_apply_suffix(root, ft_free_node);
		root = NULL;
	}
}
