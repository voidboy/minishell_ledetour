#include "minishell.h"

int	ft_dbl_quoting(char *str, int *i, char *newstr)
{
	int	len;

	len = 0;
	if (str[*i] && str[*i] == '\"')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\"')
		{
			if (str[*i] && str[*i] == '\\' && (str[*i+1] == '\\' || \
						str[*i+1] == '\"' || str[*i+1] == '\'' || str[*i+1] == '$'))
				(*i)++;
			if (str[*i])
			{
				if (newstr)
					*(newstr + len) = str[*i];
				len++;
				(*i)++;
			}
		}
		if (str[*i])
			(*i)++;
	}
	return (len);
}

int	ft_simple_quoting(char *str, int *i, char *newstr)
{
	int	len;

	len = 0;
	if (str[*i] && str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] && str[*i] != '\'')
		{
			if (newstr)
				*(newstr + len) = str[*i];
			len++;
			(*i)++;
		}
		if (str[*i])
			(*i)++;
	}
	return (len);
}

int	ft_backslash(char *str, int *i, char *newstr)
{
	int	len;

	len = 0;
	while (str[*i] && str[*i] == '\\')
	{
		(*i)++;
		if (str[*i])
		{
			if (newstr)
				*(newstr + len) = str[*i];
			len++;
			(*i)++;
		}
	}
	return (len);
}

int	ft_quoting(char *str, int *i, char *newstr)
{
	int	len;
	int	save_i;
	int	w;

	len = 0;
	w = 0;
	if (newstr)
		w = 1;
	save_i = -1;
	while (save_i != *i && str && str[*i])
	{
		save_i = *i;
		len += ft_backslash(str, i, newstr);
		newstr += len * w;
		len += ft_dbl_quoting(str, i, newstr);
		newstr += len * w;
		len += ft_simple_quoting(str, i, newstr);
		newstr += len * w;
	}
	return (len);
}

int	ft_len_sanitize(char *str)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (str[i])
	{
		len += ft_quoting(str, &i, NULL);
		if (str[i])
		{
			i++;
			len++;
		}
	}
	return (len);
}

char	*ft_sanitize(char *str)
{
	int		len;
	char	*newstr;
	int		i;

	if (!str)
		return (NULL);
	len = ft_len_sanitize(str);
	newstr = malloc(sizeof(char) * (len + 1));
	if (!newstr)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	newstr[len] = 0;
	len = 0;
	i = 0;
	while (str[i])
	{
		len += ft_quoting(str, &i, newstr + len);
		if (str[i])
			newstr[len++] = str[i++];
	}
	free(str);
	return (newstr);
}
