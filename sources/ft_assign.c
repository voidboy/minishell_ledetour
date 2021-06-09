#include "minishell.h"

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
	return i;
}

int	ft_get_varvalue(char *str, int i)
{
	while (ft_quoting(str, &i, NULL) >= 0 && str[i])
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			   break;
		i++;
	}
	return i;
}

void	ft_find_assign(char *str, t_list **lstassign, int index)
{
	t_index_var	*ivar;

	while (str[index] && (str[index] == ' ' || str[index] == '\t'))
		index++;
	if (!ft_isalpha(str[index]) && str[index++] != '_')
		return ;
	while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
		index++;
	if (str[index] == '=')
	{
		ivar = malloc(sizeof(t_index_var) * 1);
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

void ft_show_lstassign(void *content)
{

	t_index_var *ivar;

	ivar = content;
	//printf("{%d - %d - %d}\n", ivar->key, ivar->equal, ivar->value);
}

int	 ft_len_extact_assign(char *str, t_list *lstassign)
{
	int len;
	t_index_var *ivar;

	len = 0;
	while (lstassign)
	{
		ivar = lstassign->content;
		len += ivar->value - ivar->key;
		lstassign = lstassign->next;
	}
	return (ft_strlen(str) - len);
}

int	ft_extract_assign(t_btree *node, t_list *lstassign, t_var *vars)
{
	char *newstr;
	t_index_var *ivar;
	int len;
	int	i;
	int j;

	len = ft_len_extact_assign(node->cmd, lstassign);
	newstr = malloc(sizeof(char) * len + 1);
	newstr[len] = 0;
	i = 0;
	j = 0;
	len = 0;
	while (lstassign)
	{
		ivar = lstassign->content;
		vars[j].key = ft_substr(node->cmd, ivar->key, ivar->equal - ivar->key);
		vars[j].value = ft_substr(node->cmd, ivar->equal + 1, ivar->value - ivar->equal - 1);
		j++;
		while (node->cmd[i] && i < ivar->key)
			newstr[len++]= node->cmd[i++];
		i = ivar->value;
		lstassign = lstassign->next;
	}
	while (node->cmd[i])
		newstr[len++]= node->cmd[i++];
	free(node->cmd);
	node->cmd = newstr;
	return 0;
}

void ft_free_lstassign(void *ivar)
{
	free(ivar);
}

int	ft_apply_assign(t_var *vars,t_dico *dico, int nbvars)
{
	int i;

	i = -1;
	while (++i < nbvars)
		ft_set_dico_value(vars[i].key, ft_sanitize(ft_expander(vars[i].value, dico)), LOCAL, dico);
	return 0;
}

int ft_free_vars(t_var *var, int len, int r)
{
	int i;

	i = -1;
	while (r && ++i < len)
	{
		free(var[i].key);
		free(var[i].value);
	}
	free(var);
	return 0;
}

int ft_assign(t_btree *node, t_dico *dico)
{
	t_list *lstassign;
	t_var	*vars;
	int		len;
	int		i;

	lstassign = NULL;
	vars = NULL;
	ft_find_assign(node->cmd, &lstassign, 0);
	len = ft_lstsize(lstassign);
	vars = malloc(sizeof(t_var) * len);
	ft_extract_assign(node, lstassign, vars);
	node->cmd = ft_expander(node->cmd, dico);
	node->argv = ft_break(node->cmd, " \t\n"); 
	i= -1;
	while (node->argv[++i])
		node->argv[i] = ft_sanitize(node->argv[i]);
	free(node->cmd);
	node->cmd = ft_strdup(node->argv[0]);
	ft_lstiter(lstassign, ft_show_lstassign);
	ft_lstclear(&lstassign, ft_free_lstassign);
	if (node->cmd == NULL)
		ft_apply_assign(vars, dico, len);
	ft_free_vars(vars, len, !(node->cmd == NULL));
	return 0;
}
