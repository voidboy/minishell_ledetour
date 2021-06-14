#include "minishell.h"

char	*ft_escape_exp(char *str)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = 0;
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

static t_bool	is_expension(char *str, int i)
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

static void	dollar_exception1(char *src, int *i, t_context c)
{
	if (is_context_free(c))
		if (src[*i] == '$' && (src[*i + 1] == '\'' || src[*i + 1] == '"'))
			(*i)++;
}

static void	dollar_exception2(char **expanded, int *i, int *j)
{
	ft_strcpy(expanded[(*i)++], "$?");
	(*j) += 2;
}

static int	count_expension(char *str)
{
	t_context	c;
	int			i;
	int			expension_counter;
	int			commandes_counter;

	init_context(&c);
	i = 0;
	expension_counter = 0;
	commandes_counter = 0;
	while (str[i])
	{
		if (is_expension(str, i) && ++expension_counter)
		{
			if (str[i] == '~' || str[++i] == '?')
				i++;
			else
				while (str[++i] == '_' || ft_isalnum(str[i]))
					;
		}
		else if (++commandes_counter)
			while (str[i] && (!is_econtext_free(c)
					|| !is_expension(str, i)))
				update_context(&c, str[i++]);
	}
	return (expension_counter + commandes_counter);
}

static void	find_next_word(char *dst, char *src, int *i)
{
	t_context	c;
	int			j;
	t_bool		e;

	init_context(&c);
	j = 0;
	if (is_expension(src, *i))
		e = TRUE;
	else
		e = FALSE;
	while (src[*i])
	{
		dollar_exception1(src, i, c);
		if (e && j == 1 && !(src[*i] == '_' || ft_isalpha(src[*i])))
			break ;
		else if (e && j && !(src[*i] == '_' || ft_isalnum(src[*i])))
			break ;
		else if (!e)
			if (is_expension(src, *i) && is_econtext_free(c))
				break ;
		update_context(&c, src[*i]);
		dst[j++] = src[(*i)++];
	}
	dst[j] = '\0';
}

static char	**split_on_expension(char *str, int ec)
{
	char	**expanded;
	int		i;
	int		j;

	expanded = malloc(sizeof(char *) * (ec + 1));
	if (expanded == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (i < ec)
	{
		expanded[i] = malloc(sizeof(char) * (ft_strlen(str) + 1));
		if (expanded[i] == NULL)
		{
			while (i >= 0)
				free(expanded[--i]);
			return (NULL);
		}
		if (str[j] == '$' && str[j + 1] == '?')
			dollar_exception2(expanded, &i, &j);
		else
			find_next_word(expanded[i++], str, &j);
	}
	expanded[i] = NULL;
	return (expanded);
}

static void	replace_expension(char **expanded, t_dico *dico)
{
	char	*value;
	int		i;

	i = -1;
	while (expanded[++i])
	{
		if (is_expension(expanded[i], 0))
		{
			if (expanded[i][0] == '~')
				value = ft_get_dico_value("HOME", dico);
			else
				value = ft_get_dico_value(expanded[i] + 1, dico);
			free(expanded[i]);
			expanded[i] = ft_escape_exp(value);
		}
		else
			expanded[i] = expanded[i];
	}
}

char	*ft_expander(char *str, t_dico *vars)
{
	char	*str_expanded;
	char	*cpy_expanded;
	char	**expanded;
	int		expension_counter;
	int		i;

	if (!str)
		return (NULL);
	expension_counter = count_expension(str);
	expanded = split_on_expension(str, expension_counter);
	replace_expension(expanded, vars);
	str_expanded = NULL;
	i = 0;
	while (expension_counter--)
	{
		cpy_expanded = str_expanded;
		str_expanded = ft_strjoin(str_expanded, expanded[i]);
		free(expanded[i]);
		free(cpy_expanded);
		i++;
	}
	free(str);
	free(expanded);
	return (str_expanded);
}
