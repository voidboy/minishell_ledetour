#include "minishell.h"

static t_bool is_expension(char *str)
{
	if (*str == '~')
		return (TRUE);
	if (*str == '$' && (*++str == '_'|| ft_isalpha(*str) || *str == '?'))
		return (TRUE);
	else 
		return (FALSE);
}

static int count_expension(char *str)
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
		if (is_expension(&str[i]) && ++expension_counter)
		{
			if (str[i] == '~' || str[++i] == '?')
				i++;
			else 
				while (str[++i] == '_'|| ft_isalnum(str[i]))
					;
		}
		else if (++commandes_counter) 
			while (str[i] && (!is_econtext_free(c)
				|| !is_expension(&str[i])))
				update_context(&c, str[i++]);
	}
	return (expension_counter + commandes_counter);
}

static void handle_special(char *dst, char *src, int *i)
{
	int	j;

	j = 0;
	if (src[*i] == '$' && src[*i + 1])
	{
		dst[j++] = src[(*i)++];
		dst[j++] = src[(*i)++];
	}
	else if (src[*i] == '~')
		dst[j++] = src[(*i)++];
	dst[j] = '\0';
}

static void find_next_word(char *dst, char *src, int *i)
{
	t_context	c;
	int			j;
	t_bool		e;

	init_context(&c);
	j = 0;
	if (is_expension(&src[*i]))
		e = TRUE;
	else 
		e = FALSE;
	while (src[*i])
	{
		/* we must handle the context there */
		if (e && j == 1 && !(src[*i] == '_'|| ft_isalpha(src[*i])))
			break ;
		else if (e && j && !(src[*i] == '_'|| ft_isalnum(src[*i])))
			break ;
		else if (!e)
			if (is_expension(&src[*i]) && is_econtext_free(c))
				break ;
		update_context(&c, src[*i]);
		dst[j++] = src[(*i)++];
	}
	dst[j] = '\0';
}

static char **split_on_expension(char *str, int ec)
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
		if (str[j] == '~' || (str[j] == '$' && str[j + 1] == '?'))
			handle_special(expanded[i++], str, &j);
		else 
			find_next_word(expanded[i++], str, &j);
	}
	expanded[i] = NULL;
	return (expanded);
}

static void replace_expension(char **expanded, t_dico *dico)
{
	char	*value;
	int		i;

	i = -1;
	while (expanded[++i])
	{
		if (is_expension(expanded[i]))
		{
			value = ft_get_dico_value(expanded[i] + 1, dico);
			//printf("VALUE EXP is %s\n", value);
			free(expanded[i]);
			expanded[i] = value;
		}	
	}
}


char *ft_expander(char *str, t_dico *vars)
{
	char	*str_expanded;
	char	*cpy_expanded;
	char	**expanded;
	int		expension_counter;

	//printf("STR is %s", str);
	if (!str)
		return (NULL);
	expension_counter = count_expension(str);
	//printf("There are %d bloc(s)\n", expension_counter);
	expanded = split_on_expension(str, expension_counter);
	replace_expension(expanded, vars);
	str_expanded = NULL;
	while (expension_counter--)
	{
		//printf("1 STR EXPANDED IS %s\n", str_expanded);
		cpy_expanded = str_expanded;
		str_expanded = ft_strjoin(str_expanded, *expanded);
		//printf("2 STR EXPANDED IS %s\n", str_expanded);
		//free(*expanded);
		//free(cpy_expanded);
		expanded++;
	}
	//printf("STR is %s\n", str_expanded);
	free(str);
	return (str_expanded);
}
