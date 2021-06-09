/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etakouer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 14:55:40 by etakouer          #+#    #+#             */
/*   Updated: 2021/03/26 08:40:51 by etakouer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*get_str(const char *s, char c, int *pos)
{
	int		i;
	char	*str;

	i = 0;
	while (s[*pos + i] && s[*pos + i] != c)
		i++;
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s[*pos + i] && s[*pos + i] != c)
	{
		str[i] = s[*pos + i];
		i++;
	}
	str[i] = 0;
	*pos += i;
	return (str);
}

static void	*delete_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static char	**add_strtotab(char **tab, char *str)
{
	char	**new_tab;
	int		i;

	i = -1;
	while (tab[++i])
		;
	new_tab = malloc(sizeof(char *) * (i + 1 + 1));
	if (!new_tab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		new_tab[i] = tab[i];
		i++;
	}
	free(tab);
	new_tab[i] = str;
	new_tab[i + 1] = NULL;
	return (new_tab);
}

char	**ft_split(char const *s, char c)
{
	char	*str;
	char	**tab;
	int		i;

	if (!s)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * 1);
	if (!tab)
		return (NULL);
	tab[0] = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			str = get_str(s, c, &i);
			if (!str)
				return (delete_tab(tab));
			tab = add_strtotab(tab, str);
		}
	}
	return (tab);
}
