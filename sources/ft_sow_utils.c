#include "minishell.h"

char	*ft_cmdtrim(char *s)
{
	int		start;
	int		end;
	char	*tmp;
	int		len;

	tmp = " \t";
	len = ft_strlen(s);
	if (!len)
		return (NULL);
	start = 0;
	end = len;
	while (s[start] && ft_strchr(tmp, s[start]))
		start++;
	while (end > start && ft_strchr(tmp, s[end]))
		end--;
	if (s[end] == '\\')
		end++;
	if (start == 0 && end == len - 1)
		tmp = ft_strdup(s);
	else
		tmp = ft_substr(s, start, end - start + 1);
	free(s);
	return (tmp);
}

int	ft_find_last(char *hay, char *need)
{
	int		i;
	int		len_n;
	int		len;
	int		save_i;

	save_i = -1;
	len = ft_strlen(hay);
	len_n = ft_strlen(need);
	if (!len_n)
		return (-1);
	i = 0;
	while (hay[i] && ft_quoting(hay, &i, NULL) >= 0 && hay[i] && i < len)
	{
		if (save_i < 0 && i + len_n > len)
			return (-1);
		if (!ft_strncmp(hay + i, need, len_n))
		{
			save_i = i;
			i += len_n;
		}
		else
			i++;
	}
	return (save_i);
}

int	ft_op_is_finded(t_type *op, int *index)
{
	static int	ex_index = -1;

	if (*index >= 0 || ex_index >= 0)
	{
		if (*op == AND)
		{
			ex_index = *index;
			return (0);
		}
		if (ex_index > *index)
		{
			*index = ex_index;
			*op = AND;
		}
		ex_index = -1;
		return (1);
	}
	return (0);
}
