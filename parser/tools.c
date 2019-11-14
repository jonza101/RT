/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:26:23 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/19 13:26:25 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void		error(char *str)
{
	ft_putstr(str);
	ft_putchar('\n');
	exit(1);
}

int			count_words(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void		free_splited(char **splited_str)
{
	int		i;

	i = 0;
	while (splited_str[i])
	{
		ft_strdel(&splited_str[i]);
		i++;
	}
	free(splited_str);
}

int			ft_isspace(char c)
{
	if (c == 32)
		return (1);
	return (0);
}

float		ft_atof(char *str)
{
	float	res;
	float	power;
	size_t	i;
	float	sign;

	res = 0.0;
	i = 0;
	power = 1.0;
	while (ft_isspace(str[i]))
		i++;
	sign = (str[i] == '-') ? -1 : 1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	i--;
	while (ft_isdigit(str[++i]))
		res = res * 10 + (str[i] - '0');
	if (str[i] == '.')
		i++;
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + (str[i] - '0');
		power *= 10.0;
		i++;
	}
	return (res * sign / power);
}
