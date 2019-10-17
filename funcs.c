/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 18:45:21 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/17 16:55:08 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float       ft_dot_prod(t_vec3 *a, t_vec3 *b)
{
    float dot = a->x * b->x + a->y * b->y + a->z * b->z;
    return (dot);
}

float		ft_vec_len(t_vec3 *vec)
{
	float len = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return (len);
}

int			ft_color_convert(int color, double lum)
{
	int r = ((color >> 16) & 0xFF) * lum;
	int g = ((color >> 8) & 0xFF) * lum;
	int b = (color & 0xFF) * lum;

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}
