/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 22:05:45 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/02/19 21:25:20 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3 		*ft_vec_transform(t_obj *obj, t_vec3 *normal, t_vec3 *point)			//		I DONT KNOW HOW IT WORKS
{
	obj->n_temp->x = obj->normal->x;
	obj->n_temp->y = obj->normal->y;
	obj->n_temp->z = obj->normal->z;
	obj->t_p->x = point->x - obj->c->x;
	obj->t_p->y = point->y - obj->c->y;
	obj->t_p->z = point->z - obj->c->z;

	float cos_x, cos_z;
	float alpha_x, alpha_z;

	float n_len = ft_vec_len(obj->n_temp);
	if (n_len < 1e-5f)
	{
		obj->vec_temp->x = obj->t_p->x;
		obj->vec_temp->y = obj->t_p->y;
		obj->vec_temp->z = obj->t_p->z;
		return (obj->vec_temp);
	}
	else
	{
		cos_x = (float)obj->n_temp->x / (float)n_len;
		alpha_x = (obj->n_temp->y > 0.0f) ? acosf(cos_x) : -acosf(cos_x);
	}

	float cos_alpha_x = cosf(alpha_x);
	float sin_alpha_x = sin(alpha_x);

	obj->tmp->x = cos_alpha_x;
	obj->tmp->y = sin_alpha_x;
	obj->tmp->z = 0.0f;
	obj->oc_temp->x = ft_dot_prod(obj->tmp, obj->t_p);
	obj->vec_temp->x = ft_dot_prod(obj->tmp, obj->n_temp);

	obj->tmp->x = -sin_alpha_x;
	obj->tmp->y = cos_alpha_x;
	obj->tmp->z = 0.0f;
	obj->oc_temp->y = ft_dot_prod(obj->tmp, obj->t_p);
	obj->vec_temp->y = ft_dot_prod(obj->tmp, obj->n_temp);

	obj->tmp->x = 0.0f;
	obj->tmp->y = 0.0f;
	obj->tmp->z = 1.0f;
	obj->oc_temp->z = ft_dot_prod(obj->tmp, obj->t_p);
	obj->vec_temp->z = ft_dot_prod(obj->tmp, obj->n_temp);

	cos_z = (float)obj->vec_temp->z / (float)ft_vec_len(obj->vec_temp);
	alpha_z = (obj->vec_temp->x > 0.0f) ? acosf(cos_z) : -acosf(cos_z);

	float cos_alpha_z = cosf(alpha_z);
	float sin_alpha_z = sinf(alpha_z);
	obj->tmp->x = cos_alpha_z;
	obj->tmp->y = 0.0f;
	obj->tmp->z = -sin_alpha_z;
	obj->vec_tmp->x = ft_dot_prod(obj->tmp, obj->oc_temp);

	obj->tmp->x = 0.0f;
	obj->tmp->y = 1.0f;
	obj->tmp->z = 0.0f;
	obj->vec_tmp->y = ft_dot_prod(obj->tmp, obj->oc_temp);

	obj->tmp->x = sin_alpha_z;
	obj->tmp->y = 0.0f;
	obj->tmp->z = cos_alpha_z;
	obj->vec_tmp->z = ft_dot_prod(obj->tmp, obj->oc_temp);

	return (obj->vec_tmp);
}

int			ft_sph_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	float u = ft_clamp(0.5f + (float)atan2f(normal->z, normal->x) / (float)(2.0f * CL_M_PI), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - (float)asinf(normal->y) / (float)(CL_M_PI), 0.0f, 1.0f);
	int tx = (float)u * (float)obj->txt->w;
	int ty = (float)v * (float)obj->txt->h;
	int color = obj->txt->data[ty * obj->txt->w + tx];

	return (color);
}

int			ft_plane_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	if (normal->x != 0.0f || normal->y != 0.0f)
	{
		obj->vec_temp->x = normal->y;
		obj->vec_temp->y = -normal->x;
		obj->vec_temp->z = 0.0f;
		obj->vec_temp = ft_vec_normalize(obj->vec_temp);
	}
	else
	{
		obj->vec_temp->x = 0.0f;
		obj->vec_temp->y = 1.0f;
		obj->vec_temp->z = 0.0f;
		obj->vec_temp = ft_vec_normalize(obj->vec_temp);
	}

	obj->vec_tmp = ft_cross_prod(obj->vec_tmp, normal, obj->vec_temp);
	float u = ft_clamp(0.5f + (float)fmodf(ft_dot_prod(obj->vec_temp, point), 4.0f) / 8.0f, 0.0f, 1.0f);
	float v = ft_clamp(0.5f + (float)fmod(ft_dot_prod(obj->vec_tmp, point), 4.0f) / 8.0f, 0.0f, 1.0f);
	int tx = (float)u * (float)obj->txt->w;
	int ty = (float)v * (float)obj->txt->h;
	int color = obj->txt->data[ty * obj->txt->w + tx];

	return (color);
}

int 		ft_cylinder_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	obj->vec_tmp = ft_vec_transform(obj, normal, point);
	float u = ft_clamp(0.5f + (float)atan2(obj->vec_tmp->x, obj->vec_tmp->y) / (float)(2.0f * CL_M_PI), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - modff(obj->vec_tmp->z / obj->radius * 0.25f, &v) * 0.5f, 0.0f, 1.0f);
	int tx = (float)u * (float)obj->txt->w;
	int ty = (float)v * (float)obj->txt->h;
	int color = obj->txt->data[ty * obj->txt->w + tx];

	return (color);
}

int			ft_cone_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	obj->vec_tmp = ft_vec_transform(obj, normal, point);
	float p = (float)((float)obj->vec_tmp->x / (float)obj->vec_tmp->z) / (float)tanf(obj->radius);
	float u = ft_clamp((float)((obj->vec_tmp->y > 0.0f) ? acosf(p) : 2.0f * CL_M_PI - acosf(p)) / (float)(2.0f * CL_M_PI), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - modff(obj->vec_tmp->z * 0.5f, &v) * 0.5f, 0.0f, 1.0f);
	int tx = (float)u * (float)obj->txt->w;
	int ty = (float)v * (float)obj->txt->h;
	int color = obj->txt->data[ty * obj->txt->w + tx];

	return (color);
}

t_vec3		*ft_bump_maping(t_vec3 *normal, t_img *bump_map, float u, float v)
{
	int tx = (float)u * (float)bump_map->w;
	int ty = (float)v * (float)bump_map->h;

	int left_color = (tx > 0) ? bump_map->data[ty * bump_map->w + (tx - 1)] : bump_map->data[ty * bump_map->w + tx];
	int right_color = (tx < bump_map->w - 1) ? bump_map->data[ty * bump_map->w + (tx + 1)] : bump_map->data[ty * bump_map->w + tx];
	int l_r = (left_color >> 16) & 0xFF;
	int r_r = (right_color >> 16) & 0xFF;
	float l_i = (float)l_r / 255.0f;
	float r_i = (float)r_r / 255.0f;
	float x_gradient = (l_i - r_i);

	int up_color = (ty > 0) ? bump_map->data[(ty - 1) * bump_map->w + tx] : bump_map->data[ty * bump_map->w + tx];
	int down_color = (ty < bump_map->h - 1) ? bump_map->data[(ty + 1) * bump_map->w + tx] : bump_map->data[ty * bump_map->w + tx];
	int u_r = (up_color >> 16) & 0xFF;
	int d_r = (down_color >> 16) & 0xFF;
	float u_i = (float)u_r / 255.0f;
	float d_i = (float)d_r / 255.0f;
	float y_gradient = (u_i - d_i);

	normal->x = normal->x - x_gradient - y_gradient;
	normal->y = normal->y - x_gradient - y_gradient;
	normal->z = normal->z - x_gradient - y_gradient;
	normal = ft_vec_normalize(normal);
	return (normal);
}

t_vec3		*ft_sph_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	float u = ft_clamp(0.5f + (float)atan2f(normal->z, normal->x) / (float)(2.0f * CL_M_PI), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - (float)asinf(normal->y) / (float)CL_M_PI, 0.0f, 1.0f);
	obj->uv->x = u;
	obj->uv->y = v;
	normal = ft_bump_maping(normal, obj->bump, u, v);
	return (normal);
}

t_vec3		*ft_plane_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	if (normal->x != 0.0f || normal->y != 0.0f)
	{
		obj->vec_temp->x = normal->y;
		obj->vec_temp->y = -normal->x;
		obj->vec_temp->z = 0.0f;
		obj->vec_temp = ft_vec_normalize(obj->vec_temp);
	}
	else
	{
		obj->vec_temp->x = 0.0f;
		obj->vec_temp->y = 1.0f;
		obj->vec_temp->z = 0.0f;
		obj->vec_temp = ft_vec_normalize(obj->vec_temp);
	}

	obj->vec_tmp = ft_cross_prod(obj->vec_tmp, normal, obj->vec_temp);
	float u = ft_clamp(0.5f + (float)fmodf(ft_dot_prod(obj->vec_temp, point), 4.0f) / 8.0f, 0.0f, 1.0f);
	float v = ft_clamp(0.5f + (float)fmod(ft_dot_prod(obj->vec_tmp, point), 4.0f) / 8.0f, 0.0f, 1.0f);
	obj->uv->x = u;
	obj->uv->y = v;
	normal = ft_bump_maping(normal, obj->bump, u, v);
	return (normal);
}

t_vec3 		*ft_cone_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	obj->vec_tmp = ft_vec_transform(obj, normal, point);
	float p = (float)((float)obj->vec_tmp->x / (float)obj->vec_tmp->z) / (float)tanf(obj->radius);
	float u = ft_clamp((float)((obj->vec_tmp->y > 0.0f) ? acosf(p) : 2.0f * CL_M_PI - acosf(p)) / (float)(2.0f * CL_M_PI), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - modff(obj->vec_tmp->z * 0.5f, &v) * 0.5f, 0.0f, 1.0f);
	obj->uv->x = u;
	obj->uv->y = v;
	normal = ft_bump_maping(normal, obj->bump, u, v);
	return (normal);
}

t_vec3		*ft_cylinder_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point)
{
	obj->vec_tmp = ft_vec_transform(obj, normal, point);
	float u = ft_clamp(0.5f + (float)atan2(obj->vec_tmp->x, obj->vec_tmp->y) / (float)(2.0f * CL_M_PI), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - modff(obj->vec_tmp->z / obj->radius * 0.25f, &v) * 0.5f, 0.0f, 1.0f);
	obj->uv->x = u;
	obj->uv->y = v;
	normal = ft_bump_maping(normal, obj->bump, u, v);
	return (normal);
}
