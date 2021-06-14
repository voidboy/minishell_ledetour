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
	if (n == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
    	rl_replace_line("",0); 
    	rl_redisplay();
		ft_set_dico_value(ft_strdup("?"), ft_strdup("1") , LOCAL, g_minishell.dico);
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
		echo_control_seq(FALSE);
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
		line = add_linefeed(line);
		root = ft_sow(line);
		btree_show(root);
		prove = ft_prove(root);
		ft_here_doc_open(root);
		if (prove != -1)
		{
			if (ft_cross(root, &dico) == EXIT_FORK)
				ft_set_dico_value(ft_strdup("?"), ft_strdup("1") , LOCAL, &dico);
			sig_apply(CHILD);
			while(wait(NULL)!=-1)
				;
			sig_apply(PARENT);
			echo_control_seq(FALSE);
			//btree_show(root);
			//ft_lstiter(dico.sets, ft_show_dico);
		}
		else 
			ft_set_dico_value(ft_strdup("?"), ft_strdup("258"), LOCAL, &dico);
		btree_apply_suffix(root, ft_free_node);
		root = NULL;
	}
}
