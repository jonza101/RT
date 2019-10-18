/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 16:33:02 by jkimberl          #+#    #+#             */
/*   Updated: 2019/09/14 02:14:30 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	unsigned long int	x;
	int					sign;
	int					i;

	i = 0;
	x = 0;
	sign = 1;
	i = ft_skipwhitespaces(str);
	if (str[i] == '-' || str[i] == '+')
	{
		sign = (str[i] == '-') ? -1 : 1;
		i++;
	}
	while (str[i] != '\0' && str[i] > 47 && str[i] < 58)
	{
		if (((x == MAX_LONG_INT / 10 && str[i] - '0' > 7)
			|| x > MAX_LONG_INT / 10) && sign == 1)
			return (-1);
		else if (((x == MAX_LONG_INT / 10 && str[i] - '0' > 8)
			|| x > MAX_LONG_INT / 10) && sign == -1)
			return (0);
		x = x * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(x * sign));
}
