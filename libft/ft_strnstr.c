/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etakouer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 07:48:07 by etakouer          #+#    #+#             */
/*   Updated: 2021/02/16 09:08:17 by etakouer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *need, size_t len)
{
	size_t	i;
	size_t	len_n;

	len_n = ft_strlen(need);
	if (!len_n)
		return ((char *)hay);
	i = 0;
	while (hay[i] && i < len)
	{
		if (i + len_n > len)
			return (NULL);
		if (!ft_strncmp(hay + i, need, len_n))
			return ((char *)&hay[i]);
		i++;
	}
	return (NULL);
}
