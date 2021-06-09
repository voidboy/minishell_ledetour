#include "minishell.h"

/*
#include <stdio.h>
#include <stdlib.h>
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}
*/

char	*ft_strstr(char *str, char *to_find)
{
	int i;
	int j;

	i = 0;
	while (ft_quoting(str, &i, NULL) >= 0 && str[i])
	{
		j = 0;
		while (to_find[j] && (str[i] != to_find[j]))
			j++;
		if (to_find[j])
			return (str + i - 1);
		i++;
	}
	return (str + ft_strlen(str) - 1);
}

char	**ft_add_str(char **tab_str, char *str, int size_tab)
{
	int		i;
	char	**new_tab;

	if (!(*str))
		return (tab_str);
	new_tab = malloc((size_tab + 2) * sizeof(char *));
	i = 0;
	while (i < size_tab)
	{
		new_tab[i] = tab_str[i];
		i++;
	}
	new_tab[i] = str;
	new_tab[i + 1] = 0;
	return (new_tab);
}

char	*ft_create_str(char *start, char *end)
{
	int		size;
	char	*str;
	int		i;

	i = 0;
	size = end - start + 1;
	str = malloc((size + 1) * sizeof(char));
	while (start <= end)
	{
		str[i] = *start;
		start++;
		i++;
	}
	str[i] = 0;
	return (str);
}

char	**ft_break(char *str, char *charset)
{
	int		size_tab;
	char	**tab_str;
	char	*pos_charset;
	char	*substr;

	size_tab = 0;
	tab_str = malloc(sizeof(char *));
	*tab_str = 0;
	if (!str)
		return tab_str;
	while (*str)
	{
		pos_charset = ft_strstr(str, charset);
		substr = ft_create_str(str, pos_charset);
		//printf("%s\n", substr);
		if (*substr)
		{
			tab_str = ft_add_str(tab_str, substr, size_tab);
			size_tab++;
			str = pos_charset + 1;
		}else
			str++;
	}
	return (tab_str);
}

/*
int main(void)
{
	char **strs;
	
	strs = ft_break("ls|ds"," \t\n");
	while (*strs)
	{
		printf("{%s}\n",*strs);
		strs++;
	}

}*/
