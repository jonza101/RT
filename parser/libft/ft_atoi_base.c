/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 20:58:01 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/02 20:58:03 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		read_base(char *str)
{
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
		return (16);
	else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B'))
		return (2);
	else if (str[0] == '0' && (str[1] == 'o' || str[1] == 'O'))
		return (8);
	else if (ft_isdigit(str[0]) && (!str[1] || ft_isdigit(str[1])))
		return (10);
	else
		return (0);
}

int 	is_base_char(char c, int base)
{
	const char	*base_str = "0123456789ABCDEF";
	int			i;


	i = 0;
	while (i < base)
	{
		if (c == base_str[i] || (i > 9 && c == base_str[i] + 32))
			return (i);
		i++;
	}
	return (-1);
}

int 	ft_atoi_base(char *str)
{
	int base;
	int	i;
	int result;

	i = 0;
	result = 0;
	if (!(base = read_base(str)))
		return (0);
	if (base != 10)
		i = i + 2;
	while (is_base_char(str[i], base) != -1)
	{
		result = result * base + is_base_char(str[i], base);
		i++;
	}
	return (result);
}
