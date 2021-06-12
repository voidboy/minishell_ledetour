#include "minishell.h"

void	ft_free_lstassign(void *ivar)
{
	free(ivar);
}

int	ft_free_vars(t_var *var, int len, int r)
{
	int	i;

	i = -1;
	while (r && ++i < len)
	{
		free(var[i].key);
		free(var[i].value);
	}
	free(var);
	return (0);
}

int	ft_get_varkey(char *str, int i)
{
	while (--i >= 0 && str[i] != ' ' && str[i] != '\t')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (-1);
	}
	i++;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (-1);
	return (i);
}

int	ft_get_varvalue(char *str, int i)
{
	while (ft_quoting(str, &i, NULL) >= 0 && str[i])
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			   break ;
		i++;
	}
	return (i);
}

int	ft_goto_assign(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (!ft_isalpha(str[*i]) && str[(*i)++] != '_')
		return (0);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (1);
}

void	ft_find_assign(char *str, t_list **lstassign, int index)
{
	t_index_var	*ivar;

	if (ft_goto_assign(str, &index) && str[index] == '=')
	{
		ivar = malloc(sizeof(t_index_var) * 1);
		if (!ivar)
		{
			ft_lstclear(lstassign, ft_free_lstassign);
			ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
		}
		ivar->equal = index;
		ivar->key = ft_get_varkey(str, index);
		if (ivar->key >= 0)
		{
			ivar->value = ft_get_varvalue(str, index);
			ft_lstadd_back(lstassign, ft_lstnew(ivar));
			ft_find_assign(str, lstassign, ivar->value);
		}
		else
		{
			free(ivar);
			return ;
		}
	}
}

int	ft_len_extact_assign(char *str, t_list *lstassign)
{
	int			len;
	t_index_var	*ivar;

	len = 0;
	while (lstassign)
	{
		ivar = lstassign->content;
		len += ivar->value - ivar->key;
		lstassign = lstassign->next;
	}
	return (ft_strlen(str) - len);
}

char	*ft_create_newcmd(t_btree *node, t_list *lstassign, t_var *vars)
{
	char	*newstr;
	int		len;

	len = ft_len_extact_assign(node->cmd, lstassign);
	newstr = malloc(sizeof(char) * len + 1);
	if (!newstr)
	{
		ft_lstclear(&lstassign, ft_free_lstassign);
		ft_free_vars(vars, len, !(node->cmd == NULL));
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	}
	newstr[len] = 0;
	return (newstr);
}

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
