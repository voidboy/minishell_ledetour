#include "minishell.h"

static int ft_here_doc(t_btree *node, int start, int len)
{
	//printf("node cmd is %s - %d - %d\n", node->cmd + start, start, len);
	node->delimiter = ft_substr(node->cmd, start, len);
	if (!node->delimiter)
		ft_error((const char *[]){_strerror(errno), "\n", NULL}, TRUE);
	node->delimiter = ft_sanitize(node->delimiter);
	//printf("del is [%s] : \n", delimiter);
	return (0);
}

static size_t	consume_filename(t_btree *node, int *i)
{
	t_context	c;
	size_t		consumed;
	t_way		r;

	init_context(&c);
	r = trim_redir(node->cmd, i);
	consumed = 0;
	while (node->cmd[*i])
	{
		if (is_context_free(c))
			if (is_delimiter(node->cmd[*i]))
				break ;
		update_context(&c, node->cmd[*i]);
		(*i)++;
		consumed++;
	}
	if (!is_context_free(c))
		context_error();
	else if (consumed == 0)
		parse_error(node, node->cmd[*i]);
	if (r == IN_IN && consumed)
		ft_here_doc(node, *i - consumed, consumed);
	return (consumed);
}

static int	check_redirections_syntax(t_btree *node)
{
	int			i;
	t_context	c;

	init_context(&c);
	i = 0;
	while (node->cmd[i])
	{
		if (is_context_free(c) && is_redirection(node->cmd[i]))
		{
			if (consume_filename(node, &i) == 0)
				return (ERROR);
			update_context(&c, node->cmd[i]);
		}
		else 
			update_context(&c, node->cmd[i++]);
	}
	if (!is_context_free(c))
	{
		context_error();
		return (ERROR);
	}
	else
		return (SUCCESS);
}

static int	check_node(t_btree *node)
{
	if (node->type == CMD)
		return (check_redirections_syntax(node));
	else if (node->type == NEWLINE)
		return (SUCCESS);
	else 
	{
		if (node->type == SEMICOLON && !node->parent)
		{
			if (check_left(node) == ERROR)
				return (ERROR);
			else
				return (SUCCESS);
		}
		else if (check_left(node) == ERROR || check_right(node) == ERROR)
			return (ERROR);
		else 
			return (SUCCESS);
	}
}

static int	check_tree(t_btree *root, int (*check_node)(t_btree *node))
{
	if (root)
	{
		if (check_tree(root->left, check_node) == ERROR)
			return (ERROR);
		if (check_node(root) == ERROR)
			return (ERROR);
		if (check_tree(root->right, check_node) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

int	ft_prove(t_btree *root)
{
	if (check_tree(root, check_node))
		return (ERROR);
	else
		return (SUCCESS);
}
