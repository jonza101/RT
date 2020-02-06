/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 18:38:56 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/02/07 00:07:21 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <limits.h>

t_obj	*ft_shadow_intersection(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max, t_obj *obj)
{
	mlx->closest = max;

	int i = -1;
	while (++i < mlx->obj_count)
	{
		float t = mlx->obj[i]->intersect(origin, dir, mlx->obj[i]);
		if (t >= min && t < mlx->closest && mlx->obj[i] != obj)
		{
			return (mlx->obj[i]);
		}
	}
	return (NULL);
}

t_obj	*ft_closest_intersection(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max, t_obj *refl_obj)
{
	mlx->closest = max;
	t_obj *closest_obj = NULL;

	int i = -1;
	while (++i < mlx->obj_count)
	{
		float t = mlx->obj[i]->intersect(origin, dir, mlx->obj[i]);
		if (t >= min && t < mlx->closest && refl_obj != mlx->obj[i])
		{
			mlx->closest = t;
			closest_obj = mlx->obj[i];
		}
	}
	return (closest_obj);
}

t_vec3	*ft_reflect(t_vec3 *origin, t_vec3 *normal, t_vec3 *refl_ray)
{
    refl_ray->x = 2.0f * ft_dot_prod(origin, normal) * normal->x - origin->x;
    refl_ray->y = 2.0f * ft_dot_prod(origin, normal) * normal->y - origin->y;
    refl_ray->z = 2.0f * ft_dot_prod(origin, normal) * normal->z - origin->z;
    return (refl_ray);
}

t_vec3	*ft_refract(t_vec3 *dir, t_vec3 *normal, float refractive_index, t_vec3 *refr_ray)
{
	float cosi = ft_clamp(-1.0f, 1.0f, ft_dot_prod(dir, normal));
	float etai = 1.0f;
	float etat = refractive_index;

	t_vec3 *n = normal;
	if (cosi < 0.0f)
		cosi = -cosi;
	else
	{
		float temp = etai;
		etai = etat;
		etat = temp;

		n->x = -normal->x;
		n->y = -normal->y;
		n->z = -normal->z;
	}

	float eta = (float)etai / (float)etat;
	float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
	if (k < 0.0f)
	{
		refr_ray->x	=  0.0f;
		refr_ray->y	=  0.0f;
		refr_ray->z	=  0.0f;
	}
	else
	{
		refr_ray->x = eta * dir->x + (eta * cosi - sqrtf(k)) * n->x;
		refr_ray->y = eta * dir->y + (eta * cosi - sqrtf(k)) * n->y;
		refr_ray->z = eta * dir->z + (eta * cosi - sqrtf(k)) * n->z;
	}
	

	// float n = refractive_index;
	// float cosi = -(ft_dot_prod(normal, dir));
	// t_vec3 *norm = normal;

	// if (cosi < 0.0f)
	// 	cosi = -cosi;
	// else
	// {
	// 	n = 1.0f / (float)n;
	// 	norm->x = -normal->x;
	// 	norm->y = -normal->y;
	// 	norm->z = -normal->z;
	// }
	// float sint2 = n * n * (1.0f - cosi * cosi);
	// float cost = sqrt(1.0f - sint2);

	// refr_ray->x = n * dir->x + (n * cosi - cost) * norm->x;
	// refr_ray->y = n * dir->y + (n * cosi - cost) * norm->y;
	// refr_ray->z = n * dir->z + (n * cosi - cost) * norm->z;

	return (refr_ray);
}

int		ft_trace_ray(t_mlx *mlx, t_vec3 *origin, t_vec3 *dir, float min, float max, int depth, t_obj *refl_obj)
{
	t_obj *obj = ft_closest_intersection(mlx, origin, dir, min, max, refl_obj);
	if (!obj)
		return (BACKGROUND_COLOR);
	//return (obj->color);

	mlx->point->x = origin->x + mlx->closest * dir->x;
	mlx->point->y = origin->y + mlx->closest * dir->y;
	mlx->point->z = origin->z + mlx->closest * dir->z;

	mlx->normal = obj->normal_calc(mlx->normal, dir, mlx->point, obj);

	mlx->neg_dir->x = -dir->x;
	mlx->neg_dir->y = -dir->y;
	mlx->neg_dir->z = -dir->z;

	int l_color = BACKGROUND_COLOR;
	float intensity = 0.0f;
	float s_max;
	int i = -1;
	while (++i < mlx->light_count)
	{
		if (mlx->light[i]->intensity <= 0.0f)
			continue;
		if (mlx->light[i]->type == AMBIENT_L)
			intensity += mlx->light[i]->intensity;
		else
		{
			if (mlx->light[i]->type == 1)
			{
				mlx->light_dir->x = mlx->light[i]->vec->x - mlx->point->x;
				mlx->light_dir->y = mlx->light[i]->vec->y - mlx->point->y;
				mlx->light_dir->z = mlx->light[i]->vec->z - mlx->point->z;
				// mlx->light_dir = ft_vec_normalize(mlx->light_dir);

				s_max = 1.0f;
			}
			else
				continue;


			float s_i = 0.0f;
			if (!mlx->soft_shadows)
			{
				t_obj *s_obj = ft_shadow_intersection(mlx, mlx->point, mlx->light_dir, 0.000001f, s_max, obj);
				if (s_obj)
					s_i = 1.0f - s_obj->transparency;
					//continue;
			}
			else
			{
				float x1 = mlx->point->x;
				float y1 = mlx->point->z;
				float x2 = mlx->light[i]->vec->x;
				float y2 = mlx->light[i]->vec->z;
				float len = sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				float xxx = (float)(mlx->light[i]->radius * (y1 - y2)) / (float)len;
				float yyy = (float)(mlx->light[i]->radius * (x2 - x1)) / (float)len;

				t_vec2 vec[4];
				vec[0].x = x2 + xxx;
				vec[0].y = y2 + yyy;
				vec[1].x = x2 - xxx;
				vec[1].y = y2 + yyy;
				vec[2].x = x2 + xxx;
				vec[2].y = y2 - yyy;
				vec[3].x = x2 - xxx;
				vec[3].y = y2 - yyy;

				int idx = 0;
				int f = -1;
				while (++f < 4)
				{
					float dx = vec[f].x - x2;
					float dy = vec[f].y - y2;
					float dist = sqrtf(dx * dx + dy * dy);
					if (dist == mlx->light[i]->radius)
					{
						idx = f;
						break;
					}
				}

				t_vec3 l_vec = {vec[idx].x, mlx->light[i]->vec->y + mlx->light[i]->radius, vec[idx].y};
				t_vec3 diff = {mlx->light[i]->vec->x - l_vec.x, l_vec.y, mlx->light[i]->vec->z - l_vec.z};
				t_vec3 r_vec = {mlx->light[i]->vec->x + diff.x, l_vec.y, mlx->light[i]->vec->z + diff.z};

				t_vec3 v_cell[mlx->ss_cell];
				float step = (float)(2.0f * mlx->light[i]->radius) / (float)mlx->ss_cell;

				t_vec2 temp = {r_vec.x - l_vec.x, r_vec.z - l_vec.z};
				float temp_len = sqrtf(temp.x * temp.x + temp.y * temp.y);
				t_vec2 u = {(float)temp.x / (float)temp_len, (float)temp.y / (float)temp_len};

				int c = -1;
				while (++c < mlx->ss_cell)
				{
					v_cell[c].x = l_vec.x + (step * c) * u.x;
					v_cell[c].y = l_vec.y;
					v_cell[c].z = l_vec.z + (step * c) * u.y;
				}

				// printf("px %f	pz %f\n", mlx->point->x, mlx->point->z);
				// printf("lx %f	lz %f\n", mlx->light[i]->vec->x, mlx->light[i]->vec->z)
				// printf("dist %f\n", min);
				// printf("lvx %f	lvz %f\n", l_vec.x, l_vec.z);
				// printf("rvx %f	rvz %f\n\n", r_vec.x, r_vec.z);

				float sh = 0;

				int xx = -1;
				while (++xx < mlx->ss_cell)
				{
					int yy = -1;
					while (++yy < mlx->ss_cell)
					{
						float rnd = ((float)rand() / (float)RAND_MAX) * step;

						float cell_x = v_cell[xx].x + rnd;
						float cell_y = v_cell[xx].y - (yy * step) + rnd;
						float cell_z = v_cell[xx].z + rnd;

						float dx = mlx->point->x - cell_x;
						float dy = mlx->point->y - cell_y;
						float dz = mlx->point->z - cell_z;
						float dist = sqrtf(dx * dx + dy * dy + dz * dz);

						mlx->s_dir->x = (float)(cell_x - mlx->point->x) / (float)dist;
						mlx->s_dir->y = (float)(cell_y - mlx->point->y) / (float)dist;
						mlx->s_dir->z = (float)(cell_z - mlx->point->z) / (float)dist;

						t_obj *s_o = ft_shadow_intersection(mlx, mlx->point, mlx->s_dir, 0.000001f, dist, obj);
						if (s_o)
						{
							sh++;
							sh -= s_o->transparency;
						}
					}
				}
				s_i = (float)sh / (float)(mlx->ss_cell * mlx->ss_cell);
			}


			float n_dot_l = ft_dot_prod(mlx->normal, mlx->light_dir);
			if (n_dot_l > 0.0f)
			{
				float intens = ((float)(mlx->light[i]->intensity) * (float)n_dot_l / (float)((float)ft_vec_len(mlx->normal) * (float)ft_vec_len(mlx->light_dir)));
				intens = intens * (1.0f - s_i);
				intensity += intens;

				l_color = ft_sum_color(l_color, mlx->light[i]->color, 1.0f, intens);
			}

			if (obj->specular > 0.0f)
			{
				float dot = ft_dot_prod(mlx->normal, mlx->light_dir);
				mlx->s_refl->x = 2.0f * mlx->normal->x * dot - mlx->light_dir->x;
				mlx->s_refl->y = 2.0f * mlx->normal->y * dot - mlx->light_dir->y;
				mlx->s_refl->z = 2.0f * mlx->normal->z * dot - mlx->light_dir->z;

				float r_dot_v = ft_dot_prod(mlx->s_refl, mlx->neg_dir);
				if (r_dot_v > 0.0f)
				{
					float intens = (mlx->light[i]->intensity * powf((float)r_dot_v / (float)((float)ft_vec_len(mlx->s_refl) * (float)ft_vec_len(mlx->neg_dir)), obj->specular));
					intens = intens * (1.0f - s_i);
					intensity += intens;

					l_color = ft_sum_color(l_color, mlx->light[i]->color, 1.0f, intens);
				}
			}
		}
	}

	if (intensity > 1.0f)
		intensity = 1.0f;

	if (mlx->effect_i == CEL_SHADING)
	{
		float cel_dot = ft_dot_prod(dir, mlx->normal);
		float curr_intensity = 0.0f;
		int cel_f = 0;
		int cel_i = -1;
		while (++cel_i < mlx->cel_band)
		{
			if (intensity >= curr_intensity && intensity < (curr_intensity + (1.0f / (float)mlx->cel_band)))
			{
				intensity = curr_intensity + (1.0f / (float)mlx->cel_band / 2.0f);
				cel_f = 1;
				break;
			}
			curr_intensity += (1.0f / (float)mlx->cel_band);
		}
	}

	int txt_trans = 0;
	int color = obj->color;
	if (obj->txt)
	{
		color = obj->txt_map(obj, mlx->normal, mlx->point);
		if (obj->txt_trans && color == obj->txt_ignore_color)
		{
			color = obj->color;
			txt_trans = 1;
		}
	}
	color = ft_color_lum(color, intensity);

	if (obj->mirrored > 0.0f && depth <= MAX_DEPTH && ((obj->txt && txt_trans) || (!obj->txt)))
	{
		mlx->refl_ray = ft_reflect(mlx->neg_dir, mlx->normal, mlx->refl_ray);
		int reflected_color = ft_trace_ray(mlx, mlx->point, mlx->refl_ray, 0.000001f, __FLT_MAX__, depth + 1, obj);
		color = ft_sum_color(color, reflected_color, 1 - obj->mirrored, obj->mirrored);
	}

	if (obj->transparency > 0.0f && depth <= MAX_DEPTH && ((obj->txt && txt_trans) || (!obj->txt)))
	{
		mlx->refr_ray = ft_refract(dir, mlx->normal, obj->refractive_index, mlx->refr_ray);

		int trans_color = ft_trace_ray(mlx, mlx->point, mlx->refr_ray, 0.000001f, __FLT_MAX__, depth + 1, obj);
		color = ft_sum_color(color, trans_color, 1 - obj->transparency, obj->transparency);
	}

	if (mlx->colored_light)
		color = ft_sum_color(color, l_color, 1.0 - intensity, intensity);
	return (color);
}

void	ft_render(t_mlx *mlx)
{
    int x = -W * 0.5f - 1;
	int w = W * 0.5f;
	int h = H * 0.5f;
    while (++x < w)
    {
        int y = -h - 1;
        while (++y < h)
        {
			int color;

			mlx->dir->x = (float)x / (float)W * (float)AR;
			mlx->dir->y = -(float)y / (float)H;
			mlx->dir->z = 1.0f;

			if (mlx->aa_idx == 0)
			{
				mlx->dir = ft_vec_rotate(mlx->dir, mlx->dx, mlx->dy, mlx->s_refl);
				color = ft_trace_ray(mlx, mlx->cam, mlx->dir, 1.0f, __FLT_MAX__, 0, NULL);
			}
			else
			{
				int r = 0, g = 0, b = 0;

				double aa_step = (double)mlx->pix_len / (double)mlx->aa_val[mlx->aa_idx];
				double pix_len_half = mlx->pix_len * 0.5f;
				t_vec3 aa_cell = (t_vec3){mlx->dir->x - pix_len_half, mlx->dir->y - pix_len_half, 1.0f};
				int aa_x = -1;
				while (++aa_x < mlx->aa_val[mlx->aa_idx])
				{
					int aa_y = -1;
					while (++aa_y < mlx->aa_val[mlx->aa_idx])
					{
						mlx->aa_dir->x = aa_cell.x + aa_step * aa_x;
						mlx->aa_dir->y = aa_cell.y + aa_step * aa_y;
						mlx->aa_dir->z = 1.0f;
						mlx->aa_dir = ft_vec_rotate(mlx->aa_dir, mlx->dx, mlx->dy, mlx->s_refl);

						int t_color = ft_trace_ray(mlx, mlx->cam, mlx->aa_dir, 1.0f, __FLT_MAX__, 0, NULL);

						r += ((t_color >> 16) & 0xFF);
						g += ((t_color >> 8) & 0xFF);
						b += (t_color & 0xFF);
					}
				}

				mlx->dir = ft_vec_rotate(mlx->dir, mlx->dx, mlx->dy, mlx->s_refl);
				int clr = ft_trace_ray(mlx, mlx->cam, mlx->dir, 1.0f, __FLT_MAX__, 0, NULL);
				r += ((clr >> 16) & 0xFF);
				g += ((clr >> 8) & 0xFF);
				b += (clr & 0xFF);

				int a_aa = mlx->aa_val[mlx->aa_idx] * mlx->aa_val[mlx->aa_idx] + 1;
				r = (float)r / (float)a_aa;
				g = (float)g / (float)a_aa;
				b = (float)b / (float)a_aa;

				color = ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF));
			}

			if (mlx->effect_i == SEPIA)
				color = ft_to_sepia(color);
			else if (mlx->effect_i == GRAYSCALE)
				color = ft_to_grayscale(color);
			else if (mlx->effect_i == BLACK_WHITE)
				color = ft_to_black_white(color, mlx->bw_factor);

			if (mlx->negative)
				color = ft_to_negative(color);
			if (mlx->noise)
				color = ft_to_noise(color, mlx->ns_factor);

			int xc = x + w;
			int yc = y + h;
			mlx->data[yc * W + xc] = color;
		}
    }
}
