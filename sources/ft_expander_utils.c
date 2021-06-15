#include "minishell.h"

char	*ft_escape_exp(char *str)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!str)
		return (NULL);
	while (str[++i])
		if (str[i] == '\\' || str[i] == '\"' || str[i] == '\'')
			j++;
	new = malloc(sizeof(char) * ft_strlen(str) + 1 + j);
	i = -1;
	j = -1;
	while (str && str[++i])
	{
		if (str[i] == '\\' || str[i] == '\"' || str[i] == '\'')
			new[++j] = '\\';
		new[++j] = str[i];
	}
	new[++j] = 0;
	free(str);
	return (new);
}

t_bool	is_expension(char *str, int i)
{
	if (str[i] == '~')
	{
		if (!(str[i + 1] == ' ' || str[i + 1] == '/' || !str[i + 1]
				|| str[i + 1] == '\n') || (i && str[i - 1] != ' '))
			return (FALSE);
		else
			return (TRUE);
	}
	if (str[i] == '$')
	{
		if (str[i + 1] == '\'' || str[i + 1] == '"')
			return (FALSE);
		if (str[i] == '$' && (str[i + 1] == '_'
				|| ft_isalpha(str[i + 1]) || str[i + 1] == '?'))
			return (TRUE);
	}
	return (FALSE);
}

void	dollar_exception1(char *src, int *i, t_context c)
{
	if (is_context_free(c))
		if (src[*i] == '$' && (src[*i + 1] == '\'' || src[*i + 1] == '"'))
			(*i)++;
}

void	dollar_exception2(char **expanded, int *i, int *j)
{
	ft_strcpy(expanded[(*i)++], "$?");
	(*j) += 2;
}
