/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 18:38:56 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/17 18:17:55 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float	ft_light_calc(t_mlx *mlx, t_vec3 *point, t_vec3 *normal, t_vec3 *spec_view, float specular)
{
	float intensity = 0.0f;
	int i = -1;
	while (++i < 3)		//		LIGHT COUNT
	{
		if (mlx->light[i]->intensity <= 0.0f)
			continue;
		if (mlx->light[i]->type == 0)
			intensity += mlx->light[i]->intensity;
		else
		{
			if (mlx->light[i]->type == 1)
			{
				mlx->l_vec->x = mlx->light[i]->vec->x - point->x;
				mlx->l_vec->y = mlx->light[i]->vec->y - point->y;
				mlx->l_vec->z = mlx->light[i]->vec->z - point->z;
			}
			else if (mlx->light[i]->type == 2)
			{
				mlx->l_vec->x = mlx->light[i]->vec->x;
				mlx->l_vec->y = mlx->light[i]->vec->y;
				mlx->l_vec->z = mlx->light[i]->vec->z;
			}

			float dot_l = ft_dot_prod(normal, mlx->l_vec);		//		DIFFUSION
			if (dot_l > 0.0f)
				intensity += ((float)mlx->light[i]->intensity * (float)dot_l / ((float)ft_vec_len(normal) * (float)ft_vec_len(mlx->l_vec)));

			if (specular > 0.0f)								//		SPECULAR
			{
				float s_dot = ft_dot_prod(normal, mlx->l_vec);
				mlx->s_refl->x = 2.0f * normal->x * s_dot - mlx->l_vec->x;
				mlx->s_refl->y = 2.0f * normal->y * s_dot - mlx->l_vec->y;
				mlx->s_refl->z = 2.0f * normal->z * s_dot - mlx->l_vec->z;

				float refl_dot_v = ft_dot_prod(mlx->s_refl, spec_view);
				if (refl_dot_v > 0.0f)
					intensity += (mlx->light[i]->intensity * powf(refl_dot_v / ((float)ft_vec_len(mlx->s_refl) * (float)(ft_vec_len(spec_view))), specular));
			}
		}
	}
	return (intensity);
}

void	ft_ray_intersect(t_mlx *mlx, t_sphere *sph)
{
	mlx->oc->x = mlx->cam->x - sph->center->x;
	mlx->oc->y = mlx->cam->y - sph->center->y;
	mlx->oc->z = mlx->cam->z - sph->center->z;

	float k1 = ft_dot_prod(mlx->dir, mlx->dir);
	float k2 = 2.0f * ft_dot_prod(mlx->oc, mlx->dir);
	float k3 = ft_dot_prod(mlx->oc, mlx->oc) - sph->radius * sph->radius;

	float discr = (k2 * k2) - (4.0f * k1 * k3);
	if (discr < 0.0f)
	{
		mlx->t1 = INFINITY;
		mlx->t2 = INFINITY;
		return;
	}

	mlx->t1 = (float)(-k2 + sqrtf(discr)) / (float)(2.0f * k1);
	mlx->t2 = (float)(-k2 - sqrtf(discr)) / (float)(2.0f * k1);
}

int		ft_trace_ray(t_mlx *mlx, float min, float max)
{
	float closest = INFINITY;
	t_sphere *closest_sph = NULL;

	int i = -1;
	while (++i < 3)		//		SPHERE COUNT
	{
		ft_ray_intersect(mlx, mlx->sph[i]);
		if (mlx->t1 >= min && mlx->t2 <= max && mlx->t1 < closest)
		{
			closest = mlx->t1;
			closest_sph = mlx->sph[i];
		}
		if (mlx->t2 >= min && mlx->t2 <= max && mlx->t2 < closest)
		{
			closest = mlx->t2;
			closest_sph = mlx->sph[i];
		}
	}
	if (!closest_sph)
		return (0x0);
	// return (closest_sph->color);

	mlx->l_point->x = mlx->cam->x + closest * mlx->dir->x;
	mlx->l_point->y = mlx->cam->y + closest * mlx->dir->y;
	mlx->l_point->z = mlx->cam->z + closest * mlx->dir->z;

	mlx->l_normal->x = mlx->l_point->x - closest_sph->center->x;
	mlx->l_normal->y = mlx->l_point->y - closest_sph->center->y;
	mlx->l_normal->z = mlx->l_point->z - closest_sph->center->z;

	float len = ft_vec_len(mlx->l_normal);
	mlx->l_normal->x = (float)mlx->l_normal->x / (float)len;
	mlx->l_normal->y = (float)mlx->l_normal->y / (float)len;
	mlx->l_normal->z = (float)mlx->l_normal->z / (float)len;

	mlx->s_dir->x = -mlx->dir->x;
	mlx->s_dir->y = -mlx->dir->y;
	mlx->s_dir->z = -mlx->dir->z;

	float lum = ft_light_calc(mlx, mlx->l_point, mlx->l_normal, mlx->s_dir, closest_sph->specular);
	if (lum > 1.0f)
		lum = 1.0f;

	return (ft_color_convert(closest_sph->color, lum));
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
			int color = ft_trace_ray(mlx, 1.0f, INFINITY);
			int xc = x + W / 2;
			int yc = y + H / 2;
			if (xc >= 0 && xc < W && yc >= 0 && yc < H)
				mlx->data[yc * W + xc] = color;
		}
    }
}
