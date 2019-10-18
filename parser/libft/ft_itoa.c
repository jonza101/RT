/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 15:16:11 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/14 00:06:42 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static char	*reversestr(char *new)
{
	int		l;
	int		i;
	char	c;

	i = 0;
	l = ft_strlen(new);
	while (i < l / 2)
	{
		c = new[i];
		new[i] = new[l - 1 - i];
		new[l - 1 - i] = c;
		i++;
	}
	return (new);
}

static char	*ft_convert(char *new, int n, int k)
{
	int		i;
	int		sign;

	i = 0;
	if (n < 0)
		sign = -1;
	else
		sign = 0;
	while (i < k)
	{
		new[i] = ft_abs(n % 10) + 48;
		n = n / 10;
		i++;
	}
	if (sign == -1)
	{
		new[i] = '-';
		new[i + 1] = '\0';
	}
	else
		new[i] = '\0';
	return (reversestr(new));
}

static int	ft_count_chars(int n)
{
	int		k;

	k = 1;
	while (n / 10 != 0)
	{
		n = n / 10;
		k++;
	}
	return (k);
}

char		*ft_itoa(int n)
{
	char	*new;
	int		k;

	k = ft_count_chars(n);
	if (n < 0)
		new = (char *)malloc((k + 2) * sizeof(char));
	else
		new = (char *)malloc((k + 1) * sizeof(char));
	if (!new)
		return (NULL);
	return (ft_convert(new, n, k));
}
