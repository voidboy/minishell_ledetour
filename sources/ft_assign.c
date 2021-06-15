#include "minishell.h"

void	ft_set_var(t_btree *node, t_var *var, t_index_var *ivar)
{
	var->key = ft_substr(node->cmd, ivar->key, ivar->equal - ivar->key);
	var->value = ft_substr(node->cmd, ivar->equal + 1, \
			ivar->value - ivar->equal - 1);
}

int	ft_extract_assign(t_btree *node, t_list *lstassign, t_var *vars)
{
	char		*newcmd;
	t_index_var	*ivar;
	t_counter	c;

	newcmd = ft_create_newcmd(node, lstassign, vars);
	c.i = 0;
	c.j = 0;
	c.k = 0;
	while (lstassign)
	{
		ivar = lstassign->content;
		ft_set_var(node, &(vars[c.j]), ivar);
		c.j++;
		while (node->cmd[c.i] && c.i < ivar->key)
			newcmd[c.k++] = node->cmd[c.i++];
		c.i = ivar->value;
		lstassign = lstassign->next;
	}
	while (node->cmd[c.i])
		newcmd[c.k++] = node->cmd[c.i++];
	free(node->cmd);
	node->cmd = newcmd;
	return (0);
}

int	ft_apply_assign(t_var *vars, t_dico *dico, int nbvars)
{
	int	i;

	i = -1;
	while (++i < nbvars)
		ft_set_dico_value(vars[i].key, \
				ft_sanitize(ft_expander(vars[i].value, dico)), LOCAL, dico);
	return (0);
}

void	ft_create_argv(t_btree *node, t_dico *dico)
{
	int	i;

	node->cmd = ft_expander(node->cmd, dico);
	node->argv = ft_break(node->cmd, " \t\n");
	i = -1;
	while (node->argv[++i])
		node->argv[i] = ft_sanitize(node->argv[i]);
	free(node->cmd);
	node->cmd = ft_strdup(node->argv[0]);
}

int	ft_assign(t_btree *node, t_dico *dico)
{
	t_list	*lstassign;
	t_var	*vars;
	int		len;

	lstassign = NULL;
	vars = NULL;
	ft_find_assign(node->cmd, &lstassign, 0);
	len = ft_lstsize(lstassign);
	vars = malloc(sizeof(t_var) * len);
	if (!vars)
	{
		ft_lstclear(&lstassign, ft_free_lstassign);
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	}
	ft_extract_assign(node, lstassign, vars);
	ft_create_argv(node, dico);
	ft_lstclear(&lstassign, ft_free_lstassign);
	if (node->cmd == NULL)
		ft_apply_assign(vars, dico, len);
	ft_free_vars(vars, len, !(node->cmd == NULL));
	return (0);
}
