/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 16:38:57 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/11/10 18:30:01 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float		ft_sph_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	obj->oc_temp->x = origin->x - obj->c->x;
	obj->oc_temp->y = origin->y - obj->c->y;
	obj->oc_temp->z = origin->z - obj->c->z;

	float k1 = ft_dot_prod(dir, dir);
	float k2 = 2.0f * ft_dot_prod(obj->oc_temp, dir);
	float k3 = ft_dot_prod(obj->oc_temp, obj->oc_temp) - obj->radius * obj->radius;

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (-1.0f);

	float t1 = (float)(-k2 + sqrtf(discr)) / (float)(2.0f * (float)k1);
	float t2 = (float)(-k2 - sqrtf(discr)) / (float)(2.0f * (float)k1);

	if (t1 < t2)
		return (t1);
	return (t2);
}

float		ft_plane_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	float denom = ft_dot_prod(obj->normal, dir);
	if (fabs(denom) > 0.000001f)
	{
		obj->oc_temp->x = obj->c->x - origin->x;
		obj->oc_temp->y = obj->c->y - origin->y;
		obj->oc_temp->z = obj->c->z - origin->z;

		float t = (float)ft_dot_prod(obj->oc_temp, obj->normal) / (float)(denom);
		if (t > 0.000001f)
			return (t);
	}
	return (-1.0f);
}

float		ft_cone_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	obj->oc_temp->x = origin->x - obj->c->x;
	obj->oc_temp->y = origin->y - obj->c->y;
	obj->oc_temp->z = origin->z - obj->c->z;

	float pow_ = powf(tanf(obj->radius), 2);
	float d_dot_n = ft_dot_prod(dir, obj->normal);
	float oc_dot_n = ft_dot_prod(obj->oc_temp, obj->normal);

	float k1 = ft_dot_prod(dir, dir) - (1.0f + pow_) * powf(d_dot_n, 2);
	float k2 = 2.0f * (ft_dot_prod(dir, obj->oc_temp) - (1.0f + pow_) * d_dot_n * oc_dot_n);
	float k3 = ft_dot_prod(obj->oc_temp, obj->oc_temp) - (1.0f + pow_) * powf(oc_dot_n, 2);

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.000001f)
		return (-1.0f);

	float discr_sqrt = sqrtf(discr);
	float k1_ = 2.0f * k1;

	float t1 = (float)(-k2 + discr_sqrt) / (float)k1_;
	float t2 = (float)(-k2 - discr_sqrt) / (float)k1_;

	if (t1 < t2)
		return (t1);
	return (t2);
}

float		ft_cylinder_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
    obj->oc_temp->x = origin->x - obj->c->x;
    obj->oc_temp->y = origin->y - obj->c->y;
    obj->oc_temp->z = origin->z - obj->c->z;

	float d_dot_n = ft_dot_prod(dir, obj->normal);
	float oc_dot_n = ft_dot_prod(obj->oc_temp, obj->normal);

    float k1 = ft_dot_prod(dir, dir) - pow(d_dot_n, 2);
    float k2 = 2.0f * (ft_dot_prod(dir, obj->oc_temp) - (d_dot_n * oc_dot_n));
    float k3 = ft_dot_prod(obj->oc_temp, obj->oc_temp) - pow(oc_dot_n, 2) - pow(obj->radius, 2);

    float discr = k2 * k2 - 4.0f * k1 * k3;
    if (discr < 0.000001f)
        return (-1.0f);

	float discr_sqrt = sqrtf(discr);
	float k1_ = 2.0f * k1;

    float t1 = (float)(-k2 + discr_sqrt) / (float)k1_;
    float t2 = (float)(-k2 - discr_sqrt) / (float)k1_;

    if (t1 < t2)
        return (t1);
    return (t2);
}
