/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 18:38:56 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/24 13:10:35 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_obj		*ft_closest_intersection(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max)
{
	mlx->closest= MAXFLOAT;
	t_obj *closest_obj = NULL;

	int i = -1;
	while (++i < mlx->obj_count - 0)
	{
		float t = mlx->obj[i]->intersect((void*)mlx, origin, dir, mlx->obj[i]);
		if (t >= min && t < mlx->closest)
		{
			mlx->closest = t;
			closest_obj = mlx->obj[i];
		}
	}
	return (closest_obj);
}

t_vec3 *init_t_vec3(float x, float y, float z)
{
    t_vec3 *new;

    new = (t_vec3 *)malloc(sizeof(t_vec3));
    new->x = x;
    new->y = y;
    new->z = z;
    return (new);
}

t_vec3 *reflect(t_vec3 *I, t_vec3 *N)
{
    t_vec3 *new;

    new = (t_vec3 *)malloc(sizeof(t_vec3));
    new->x = 2.0f * ft_dot_prod(I, N) * N->x - I->x;
    new->y = 2.0f * ft_dot_prod(I, N) * N->y - I->y;
    new->z = 2.0f * ft_dot_prod(I, N) * N->z - I->z;
    return (new);
}

int		ft_trace_ray(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max, int depth)
{

	t_obj *obj = ft_closest_intersection(mlx, origin, dir, min, max);
	if (!obj)
		return (BACKGROUND_COLOR);
	// return (obj->color);

	mlx->point->x = origin->x + mlx->closest * dir->x;
	mlx->point->y = origin->y + mlx->closest * dir->y;
	mlx->point->z = origin->z + mlx->closest * dir->z;

	mlx->normal = obj->normal_calc(mlx->normal, mlx->point, obj);

	mlx->neg_dir->x = -dir->x;
	mlx->neg_dir->y = -dir->y;
	mlx->neg_dir->z = -dir->z;

	float intensity = 0.0f;
	float s_max;
	int i = -1;
	while (++i < mlx->light_count)
	{
		if (mlx->light[i]->intensity <= 0.0f)
			continue;
		if (mlx->light[i]->type == 0)
			intensity += mlx->light[i]->intensity;
		else
		{
			if (mlx->light[i]->type == 1)
			{
				mlx->light_dir->x = mlx->light[i]->vec->x - mlx->point->x;
				mlx->light_dir->y = mlx->light[i]->vec->y - mlx->point->y;
				mlx->light_dir->z = mlx->light[i]->vec->z - mlx->point->z;
				s_max = 1.0f;
			}
			// else if (mlx->light[i]->type == 2)
			// {
			// 	mlx->light_dir->x = mlx->light[i]->vec->x;
			// 	mlx->light_dir->y = mlx->light[i]->vec->y;
			// 	mlx->light_dir->z = mlx->light[i]->vec->z;
			// 	s_max = MAXFLOAT;
			// }
			else
				continue;

			t_obj *s_obj = ft_closest_intersection(mlx, mlx->point, mlx->light_dir, 0.000001f, s_max);
			if (s_obj)
				continue;

			float n_dot_l = ft_dot_prod(mlx->normal, mlx->light_dir);
			if (n_dot_l > 0.0f)
				intensity += ((float)mlx->light[i]->intensity * (float)n_dot_l / (float)((float)ft_vec_len(mlx->normal) * (float)ft_vec_len(mlx->light_dir)));

			if (obj->specular > 0.0f)
			{
				float dot = ft_dot_prod(mlx->normal, mlx->light_dir);
				mlx->s_refl->x = 2.0f * mlx->normal->x * dot - mlx->light_dir->x;
				mlx->s_refl->y = 2.0f * mlx->normal->y * dot - mlx->light_dir->y;
				mlx->s_refl->z = 2.0f * mlx->normal->z * dot - mlx->light_dir->z;

				float r_dot_v = ft_dot_prod(mlx->s_refl, mlx->neg_dir);
				if (r_dot_v > 0.0f)
					intensity += (mlx->light[i]->intensity * powf((float)r_dot_v / (float)((float)ft_vec_len(mlx->s_refl) * (float)ft_vec_len(mlx->neg_dir)), obj->specular));
			}
		}
	}

	if (intensity > 1.0f)
		intensity = 1.0f;

	int color = ft_color_convert(obj->color, intensity);
	if (depth > 1 || obj->mirrored <= 0)
	    return (color);
	t_vec3 *R = reflect(mlx->neg_dir, mlx->normal);
	int reflected_color = ft_trace_ray(mlx, mlx->point, R, 1.0f, MAXFLOAT, depth + 1);
	return (ft_sum_color(color, reflected_color, 1 - obj->mirrored, obj->mirrored));
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
			mlx->dir->y = -(float)y / (float)H;
			mlx->dir->z = 1.0f;
			int color = ft_trace_ray(mlx, mlx->cam, mlx->dir, 1.0f, MAXFLOAT, 0);
			int xc = x + W / 2;
			int yc = y + H / 2;
			if (xc >= 0 && xc < W && yc >= 0 && yc < H)
				mlx->data[yc * W + xc] = color;
		}
    }
}
