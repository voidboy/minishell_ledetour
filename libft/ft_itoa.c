/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etakouer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 10:16:21 by etakouer          #+#    #+#             */
/*   Updated: 2021/03/26 08:43:41 by etakouer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr(unsigned int nbr, char *str)
{
	if (nbr >= 10)
	{
		ft_putnbr(nbr / 10, str - 1);
		ft_putnbr(nbr % 10, str);
	}
	else
		*str = nbr + '0';
}

static int	ft_nblen(int nbr)
{
	int	len;

	len = 1;
	if (nbr <= 0)
		len++;
	while (nbr)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	unsigned int	nbr;
	int				size;
	char			*str;

	size = ft_nblen(n);
	str = malloc(size);
	if (!str)
		return (NULL);
	if (n < 0)
	{
		*str = '-';
		nbr = n * (-1);
	}
	else
	{
		nbr = n;
	}
	ft_putnbr(nbr, str + size - 2);
	str[size - 1] = 0;
	return (str);
}
