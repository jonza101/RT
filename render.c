/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 18:38:56 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/19 18:42:25 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float	ft_light_calc(t_mlx *mlx, t_vec3 *point, t_vec3 *normal, t_vec3 *v, float specular)
{
	float intensity = 0.0f;
	float s_max;
	int i = -1;
	while (++i < 3)
	{
		if (mlx->light[i]->type == 0)
			intensity += mlx->light[i]->intensity;
		else
		{
			if (mlx->light[i]->type == 1)
			{
				mlx->l_vec->x = mlx->light[i]->vec->x - point->x;
				mlx->l_vec->y = mlx->light[i]->vec->y - point->y;
				mlx->l_vec->z = mlx->light[i]->vec->z - point->z;
				s_max = 1.0f;
			}
			else if (mlx->light[i]->type == 2)
			{
				mlx->l_vec->x = mlx->light[i]->vec->x;
				mlx->l_vec->y = mlx->light[i]->vec->y;
				mlx->l_vec->z = mlx->light[i]->vec->z;
				s_max = INFINITY;
			}
			else
				continue;

			t_sphere *s_sph = ft_closest_sph_intersection(mlx, point, mlx->l_vec, 0.001f, s_max);
			if (s_sph)
				continue;

			float n_dot_l = ft_dot_prod(normal, mlx->l_vec);
			if (n_dot_l > 0.0f)
				intensity += ((float)mlx->light[i]->intensity * (float)n_dot_l / (float)((float)ft_vec_len(normal) * (float)ft_vec_len(mlx->l_vec)));

			if (specular > 0.0f)
			{
				float dot = ft_dot_prod(normal, mlx->l_vec);
				mlx->s_refl->x = 2.0f * normal->x * dot - mlx->l_vec->x;
				mlx->s_refl->y = 2.0f * normal->y * dot - mlx->l_vec->y;
				mlx->s_refl->z = 2.0f * normal->z * dot - mlx->l_vec->z;

				float r_dot_v = ft_dot_prod(mlx->s_refl, v);
				if (r_dot_v > 0.0f)
					intensity += (mlx->light[i]->intensity * powf((float)r_dot_v / (float)((float)ft_vec_len(mlx->s_refl) * (float)ft_vec_len(v)), specular));
			}
		}
	}
	return (intensity);
}

void	ft_sph_ray_intersect(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, t_sphere *sph)
{
	mlx->oc->x = origin->x - sph->center->x;
	mlx->oc->y = origin->y - sph->center->y;
	mlx->oc->z = origin->z - sph->center->z;

	float k1 = ft_dot_prod(dir, dir);
	float k2 = 2.0f * ft_dot_prod(mlx->oc, dir);
	float k3 = ft_dot_prod(mlx->oc, mlx->oc) - sph->radius * sph->radius;

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
	{
		mlx->t1 = INFINITY;
		mlx->t2 = INFINITY;
		return;
	}

	mlx->t1 = (float)(-k2 + sqrtf(discr)) / (float)(2.0f * (float)k1);
	mlx->t2 = (float)(-k2 - sqrtf(discr)) / (float)(2.0f * (float)k1);
}

t_sphere	*ft_closest_sph_intersection(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max)
{
	mlx->closest = INFINITY;
	t_sphere *closest_sph = NULL;

	int i = -1;
	while (++i < 3)
	{
		ft_sph_ray_intersect(mlx, origin, dir, mlx->sph[i]);
		if (mlx->t1 >= min && mlx->t1 < max && mlx->t1 < mlx->closest)
		{
			mlx->closest = mlx->t1;
			closest_sph = mlx->sph[i];
		}
		if (mlx->t2 >= min && mlx->t2 < max && mlx->t2 < mlx->closest)
		{
			mlx->closest = mlx->t2;
			closest_sph = mlx->sph[i];
		}
	}
	return (closest_sph);
}

int		ft_trace_ray(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max)
{
	t_sphere *sph = ft_closest_sph_intersection(mlx, origin, dir, min, max);
	if (!sph)
		return (BACKGROUND_COLOR);

	mlx->point->x = origin->x + mlx->closest * dir->x;
	mlx->point->y = origin->y + mlx->closest * dir->y;
	mlx->point->z = origin->z + mlx->closest * dir->z;

	mlx->normal->x = mlx->point->x - sph->center->x;
	mlx->normal->y = mlx->point->y - sph->center->y;
	mlx->normal->z = mlx->point->z - sph->center->z;

	float normal_len = ft_vec_len(mlx->normal);
	mlx->normal->x = (float)mlx->normal->x / (float)normal_len;
	mlx->normal->y = (float)mlx->normal->y / (float)normal_len;
	mlx->normal->z = (float)mlx->normal->z / (float)normal_len;

	mlx->s_dir->x = -dir->x;
	mlx->s_dir->y = -dir->y;
	mlx->s_dir->z = -dir->z;

	float lum = ft_light_calc(mlx, mlx->point, mlx->normal, mlx->s_dir, sph->specular);
	if (lum > 1.0f)
		lum = 1.0f;
	int color = ft_color_convert(sph->color, lum);
	return (ft_color_convert(sph->color, lum));
}

void	ft_render(t_mlx *mlx)
{
    int x = -W / 2 - 1;
    while (++x < W / 2)
    {
        int y = -H / 2 - 1;
        while (++y < H / 2)
        {
			mlx->dir->x = (float)x / (float)W * (float)AR;
			mlx->dir->y = (float)y / (float)H;
			mlx->dir->z = 1.0f;
			int color = ft_trace_ray(mlx, mlx->cam, mlx->dir, 1.0f, INFINITY);
			int xc = x + W / 2;
			int yc = y + H / 2;
			if (xc >= 0 && xc < W && yc >= 0 && yc < H)
				mlx->data[yc * W + xc] = color;
		}
    }
}
