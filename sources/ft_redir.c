#include "minishell.h"

static int add_redir(t_btree *node, char *filename, t_way way)
{
	t_redir	*new_redir;
	t_list	*new_list;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (ft_error((const char *[]){_strerror(errno), NULL}));
	new_list = malloc(sizeof(t_list));
	if (!new_list)
	{
		free(new_redir);
		return (ft_error((const char *[]){_strerror(errno), NULL}));
	}
	new_redir->way = way;
	new_redir->filename = filename;
	new_list->content = new_redir;
	new_list->next = NULL;
	ft_lstadd_back(&node->redir, new_list);
	return (SUCCESS);
}

static int extract_file(t_btree *node, int *i)
{
	char		*filename;
	int			j;
	t_context	c;
	t_way		redirT;

	init_context(&c);
	filename = malloc(sizeof(char) * ft_strlen(node->cmd) + 1);
	if (!filename)
		return (ft_error((const char *[]){_strerror(errno), NULL}));
	redirT = trim_redir(node->cmd, i);
	j = 0;
	while (node->cmd[*i])
	{
		if (is_context_free(c))
			if (is_delimiter(node->cmd[*i]))
				break ; 
		filename[j++] = node->cmd[*i];
		update_context(&c, node->cmd[*i]);
		*i = *i + 1;
	}
	filename[j] = '\0';
	return (add_redir(node, filename, redirT));
}

int ft_redir(t_btree *node)
{	
	int			i;
	int			j;
	char		*cmd_wo_redir;
	t_context	c;

	init_context(&c);
	j = 0;
	i = 0;
	/* MKO */
	cmd_wo_redir = malloc(sizeof(char) * ft_strlen(node->cmd) + 1);
	if (!cmd_wo_redir)
		return (ft_error((const char *[]){_strerror(errno), NULL}));
	while (node->cmd[i])
	{
		if (is_context_free(c) && is_redirection(node->cmd[i]))
		{
			if (extract_file(node, &i) == ERROR)
				break ;
			update_context(&c, node->cmd[i]);
		}
		else 
		{
			update_context(&c, node->cmd[i]);
			cmd_wo_redir[j++] = node->cmd[i++];
		}
	}
	cmd_wo_redir[j] = '\0';
	free(node->cmd);
	node->cmd = cmd_wo_redir;
	//printf("node->cmd is %s\n", node->cmd);
	if (node->cmd[i] != '\0')
		return (ERROR);
	else 
		return (SUCCESS);
}
