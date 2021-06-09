/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etakouer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 07:06:38 by etakouer          #+#    #+#             */
/*   Updated: 2021/03/26 07:47:11 by etakouer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1)
		i = ft_strlen(s1);
	if (s2)
		j = ft_strlen(s2);
	str = malloc(i + j + 1);
	if (!str)
		return (NULL);
	j = -1;
	i = -1;
	while (s1 && s1[++i])
		str[++j] = s1[i];
	i = -1;
	while (s2 && s2[++i])
		str[++j] = s2[i];
	str[++j] = 0;
	return (str);
}
