/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:22:37 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/19 13:22:40 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void		get_color(char *str, int *r, int *g, int *b)
{
	char	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 5)
		error("invalid map");
	*r = ft_atoi(splited_str[2]);
	*g = ft_atoi(splited_str[3]);
	*b = ft_atoi(splited_str[4]);
	free_splited(splited_str);
}

int			get_type_o(char *str)
{
	int		type;
	char	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
		error("invalid map");
	if (!(ft_strcmp(splited_str[2], "plane")))
		type = 0;
	else if (!(ft_strcmp(splited_str[2], "sphere")))
		type = 1;
	else if (!(ft_strcmp(splited_str[2], "cylinder")))
		type = 2;
	else if (!(ft_strcmp(splited_str[2], "cone")))
		type = 3;
	else
		error("invalid type of light");
	free_splited(splited_str);
	return (type);
}

t_point		*get_point(char *str)
{
	t_point	*point;
	char	**splited_str;

	point = (t_point *)malloc(sizeof(t_point));
	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 5)
		error("invalid map");
	point->x = ft_atof(splited_str[2]);
	point->y = ft_atof(splited_str[3]);
	point->z = ft_atof(splited_str[4]);
	free_splited(splited_str);
	return (point);
}

int			get_type_l(char *str)
{
	int		type;
	char	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
		error("invalid map");
	if (!(ft_strcmp(splited_str[2], "ambient")))
		type = 0;
	else if (!(ft_strcmp(splited_str[2], "point")))
		type = 1;
	else if (!(ft_strcmp(splited_str[2], "directional")))
		type = 2;
	else
		error("invalid type of light");
	free_splited(splited_str);
	return (type);
}

float		get_float(char *str)
{
	float	intensity;
	char	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
		error("invalid map");
	intensity = atof(splited_str[2]);
	free_splited(splited_str);
	return (intensity);
}
