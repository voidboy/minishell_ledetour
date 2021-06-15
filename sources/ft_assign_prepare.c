#include "minishell.h"

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
