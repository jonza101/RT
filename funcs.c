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

float		ft_min(float a, float b)
{
	if (b < a)
		return (b);
	return (a);
}

float		ft_max(float a, float b)
{
	if (b > a)
		return (b);
	return (a);
}

float	ft_clamp(float a, float min, float max)
{
	return (ft_min(ft_max(a, min), max));
}

float       ft_dot_prod(t_vec3 *a, t_vec3 *b)
{
    float dot = a->x * b->x + a->y * b->y + a->z * b->z;
    return (dot);
}

t_vec3		*ft_cross_prod(t_vec3 *vec, t_vec3 *a, t_vec3 *b)
{
	vec->x = a->y * b->z - a->z * b->y;
	vec->y = a->z * b->x - a->x * b->z;
	vec->z = a->x * b->y - a->y * b->x;
	return (vec);
}

float		ft_vec_len(t_vec3 *vec)
{
	float len = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return (len);
}

t_vec3		*ft_vec_normalize(t_vec3 *vec)
{
	float len = ft_vec_len(vec);
	vec->x = (float)vec->x / (float)len;
	vec->y = (float)vec->y / (float)len;
	vec->z = (float)vec->z / (float)len;
	return (vec);
}

int			ft_color_convert(int color, float lum)
{
	int r = ((color >> 16) & 0xFF) * lum;
	int g = ((color >> 8) & 0xFF) * lum;
	int b = (color & 0xFF) * lum;

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int         ft_sum_color(int c1, int c2, float k1, float k2)
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

int			ft_to_sepia(int color)
{
	int r = (color >> 16) & 0xFF;
	int g = (color >> 8) & 0xFF;
	int b = color & 0xFF;

	int rr = (r * 0.393f) + (g * 0.769f) + (b * 0.189f);
	int gg = (r * 0.349f) + (g * 0.686f) + (b * 0.168f);
	int bb = (r * 0.272f) + (g * 0.534f) + (b * 0.131f);

	rr = (rr > 255) ? 255 : rr;
	gg = (gg > 255) ? 255 : gg;
	bb = (bb > 255) ? 255 : bb;

	return (((rr & 0xFF) << 16) + ((gg & 0xFF) << 8) + ((bb & 0xFF)));
}

int			ft_to_grayscale(int color)
{
	int r = (color >> 16) & 0xFF;
	int g = (color >> 8) & 0xFF;
	int b = color & 0xFF;

	int rr = (r * 0.299f) + (g * 0.587f) + (b * 0.114f);
	int gg = (r * 0.299f) + (g * 0.587f) + (b * 0.114f);
	int bb = (r * 0.299f) + (g * 0.587f) + (b * 0.114f);

	rr = (rr > 255) ? 255 : rr;
	gg = (gg > 255) ? 255 : gg;
	bb = (bb > 255) ? 255 : bb;

	return (((rr & 0xFF) << 16) + ((gg & 0xFF) << 8) + ((bb & 0xFF)));
}		
