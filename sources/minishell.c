#include "minishell.h"

t_minishell g_minishell;

void sig_apply(t_stage stage)
{
	void (*ptr_hendler)(int n);
	void **fcts;

	fcts = (void *[]){&sig_hand_parent, &sig_hand_here, &sig_hand_child};
	ptr_hendler = fcts[stage];
	signal(SIGINT, ptr_hendler);
	signal(SIGQUIT, ptr_hendler);
}

void sig_hand_child(int n)
{
	echo_control_seq(TRUE);
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
    	rl_replace_line("",0); 
	}
	if (n == SIGQUIT)
	{
		rl_on_new_line();
	}
}

void sig_hand_here(int n)
{
	echo_control_seq(FALSE);
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
    	rl_replace_line("",0);
		close(STDIN_FILENO);
	}
	if (n == SIGQUIT)
	{
		rl_on_new_line();
    	rl_redisplay();
	}
}

void sig_hand_parent(int n)
{
	echo_control_seq(FALSE);
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
    	rl_replace_line("",0); 
    	rl_redisplay();
	}
	if (n == SIGQUIT)
	{
		rl_on_new_line();
    	rl_redisplay();
	}
}

int main(int ac, char **argv, char **envp)
{
	(void)ac;
	char	*line;
	t_btree	*root;
	t_dico	dico;
   	int		prove;

	(void)argv;
	dico.sets = NULL;
	dico.envp = NULL;
	root = NULL;
	echo_control_seq(FALSE);
	sig_apply(PARENT);
	ft_set_dico(&dico, envp);
	while (1)
	{
		line = readline("minishell> ");
	//	printf("\nline : {%s}\n\n", line);
		if (!line)
		{
			/* we should free here */
			write(STDOUT_FILENO, "exit\n", 5);
			exit(0);
		}
		if (ft_strlen(line))
			add_history(line);
		root = ft_sow(line);
		//btree_show(root);
		prove = ft_prove(root);
		ft_here_doc_open(root);
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
