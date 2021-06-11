#include "minishell.h"

char *ft_cmdtrim(char *s)
{
	int		start;
	int		end;
	char	*tmp;
	int		len;

	tmp = " \t";
	len = ft_strlen(s); 
	if (!len)
		return (NULL);
	start = 0;
	end = len;
	while (s[start] && ft_strchr(tmp, s[start]))
		start++;
	while (end > start && ft_strchr(tmp, s[end]))
		end--;
	if (s[end] == '\\')
		end++;
	if (start == 0 && end == len - 1)
		tmp = ft_strdup(s);
	else
		tmp = ft_substr(s, start, end - start + 1);
	free(s);
	return (tmp);
}

int	ft_find_last(char *hay, char *need)
{
	int		i;
	int		len_n;
	int		len;
	int		save_i;

	save_i = -1;
	len = ft_strlen(hay);
	len_n = ft_strlen(need);
	if (!len_n)
		return (-1);
	i = 0;
	while (hay[i] && ft_quoting(hay, &i, NULL) >= 0 && hay[i] && i < len)
	{
		if (save_i < 0 && i + len_n > len)
			return (-1);
		if (!ft_strncmp(hay + i, need, len_n))
		{
			save_i = i;
			i += len_n;
		}
		else
			i++;
	}
	return (save_i);	
}

int ft_op_is_finded(t_type *op,int *index)
{
	static int ex_index = -1;

	if (*index >= 0 || ex_index >= 0)
	{
		if (*op == AND)
		{
			ex_index = *index;
			return 0;
		}
		if (ex_index > *index)
		{
			*index = ex_index;
			*op = AND;
		}
		ex_index = -1;
		return (1);
	}
	return (0);
}

t_type	ft_split_op(char *line, char **s_op, char **s_left, char **s_right)
{
	t_type	op;
	int		index;
	char	**opstrs;

	opstrs = (char *[5]){";", "&&", "||", "|", NULL};
	op = -1;
	index = -1;
	while (opstrs[++op])
	{
		index = ft_find_last(line, opstrs[op]);
		if (ft_op_is_finded(&op, &index))
			break ;
	}
	*s_op = ft_strdup(opstrs[op]);
	*s_left = NULL;
	*s_right = NULL;
	if (op != CMD)
	{
		*s_left = ft_cmdtrim(ft_substr(line, 0, index));
		if (op == AND || op == OR)
			index++;
		index++;
		*s_right = ft_cmdtrim(ft_substr(line, index, ft_strlen(line) - index));
	}
	return (op);
}

t_btree	*btree_new_node(char *cmd, t_btree *parent, t_side side, t_type op)
{
	t_btree *node;
	static int id;

	node = (t_btree *)malloc(sizeof(t_btree));
	if (!node)
		ft_error((t_strs){_strerror(errno),"\n", NULL}, 1);
	node->cmd = cmd;
	node->parent = parent;
	node->left = 0;
	node->right = 0;
	node->id = id++;
	node->argv = NULL;
	node->side = side;
	node->redir = NULL;
	node->type = op;
	if (node->cmd[0]=='\n')
		node->type = NEWLINE;
	node->redir = NULL;
	node->fd[0] = STDIN_FILENO;
	node->fd[1] = STDOUT_FILENO;
	node->buff = NULL;
	node->delimiter = NULL;
	return (node);
}

t_btree *ft_wrap_sow(char *line, t_btree *parent, t_side side)
{
	char	*strs[3];
	t_btree *root;
	t_type op;

	if (!line)
		return NULL;
	op = ft_split_op(line, &strs[0], &strs[1], &strs[2]);
	if (op != CMD)
	{
		free(line);
		root = btree_new_node(strs[0], parent, side, op);
		root->left = ft_wrap_sow(strs[1], root, LEFT);
		root->right = ft_wrap_sow(strs[2], root, RIGHT);
		return (root);
	}
	else
		return (btree_new_node(line, parent, side, CMD));
}

t_btree *ft_sow(char *line)
{
	g_minishell.root = ft_wrap_sow(line, NULL, ROOT);
	return (g_minishell.root);
}
