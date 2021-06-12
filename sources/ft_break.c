#include "minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

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

	new_tab = malloc((size_tab + 2) * sizeof(char *));
	if (!new_tab)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	i = 0;
	while (i < size_tab)
	{
		new_tab[i] = tab_str[i];
		i++;
	}
	new_tab[i] = str;
	new_tab[i + 1] = 0;
	free(tab_str);
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
	if (!str)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
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
	if (!tab_str)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	*tab_str = 0;
	while (str && *str)
	{
		pos_charset = ft_strstr(str, charset);
		substr = ft_create_str(str, pos_charset);
		if (*substr)
		{
			tab_str = ft_add_str(tab_str, substr, size_tab);
			size_tab++;
			str = pos_charset + 1;
			continue ;
		}
		free(substr);
		str++;
	}
	return (tab_str);
}
