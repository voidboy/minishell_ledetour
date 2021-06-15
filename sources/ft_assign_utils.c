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
