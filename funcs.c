/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 18:45:21 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/28 23:57:07 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double		ft_max(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

double       ft_dot_prod(t_vec3 *a, t_vec3 *b)
{
    double dot = a->x * b->x + a->y * b->y + a->z * b->z;
    return (dot);
}

double		ft_vec_len(t_vec3 *vec)
{
	double len = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return (len);
}

t_vec3		*ft_vec_normalize(t_vec3 *vec)
{
	double len = ft_vec_len(vec);
	vec->x = (double)vec->x / (double)len;
	vec->y = (double)vec->y / (double)len;
	vec->z = (double)vec->z / (double)len;
	return (vec);
}

int			ft_color_convert(int color, double lum)
{
	int r = ((color >> 16) & 0xFF) * lum;
	int g = ((color >> 8) & 0xFF) * lum;
	int b = (color & 0xFF) * lum;

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int         ft_sum_color(int c1, int c2, double k1, double k2)
{
    int r1 = (c1 >> 16) & 0xFF;
    int g1 = (c1 >> 8) & 0xFF;
    int b1 = c1 & 0xFF;
    int r2 = (c2 >> 16) & 0xFF;
    int g2 = (c2 >> 8) & 0xFF;
    int b2 = c2 & 0xFF;
    int r = r1 * k1 + r2 * k2;
    int g = g1 * k1 + g2 * k2;
    int b = b1 * k1 + b2 * k2;
    return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}