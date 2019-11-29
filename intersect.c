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

	if ((t1 < t2 && t1 >= 0.0f) || (t1 >= 0.0f && t2 < 0.0))
		return (t1);
	if ((t2 < t1 && t2 >= 0.0f) || (t2 >= 0.0f && t1 < 0.0f))
		return (t2);
	return (-1.0f);
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

float		ft_triangle_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	float area2 = ft_vec_len(obj->normal);

	float n_dot_raydir = ft_dot_prod(obj->normal, dir);
	if (fabs(n_dot_raydir) < 0.000001f)
		return (-1.0f);

	float d = ft_dot_prod(obj->normal, obj->p0);

	float t = (float)(ft_dot_prod(obj->normal, origin) + d) / (float)n_dot_raydir;
	if (t < 0.0f)
		return (-1.0f);

	obj->t_p->x = origin->x + t * dir->x;
	obj->t_p->y = origin->y + t * dir->y;
	obj->t_p->z = origin->z + t * dir->z;


	obj->oc_temp->x = obj->p1->x - obj->p0->x;
	obj->oc_temp->y = obj->p1->y - obj->p0->y;
	obj->oc_temp->z = obj->p1->z - obj->p0->z;

	obj->vec_temp->x = obj->t_p->x - obj->p0->x;
	obj->vec_temp->y = obj->t_p->y - obj->p0->y;
	obj->vec_temp->z = obj->t_p->z - obj->p0->z;

	obj->vec_tmp = ft_cross_prod(obj->vec_tmp, obj->oc_temp, obj->vec_temp);
	if (ft_dot_prod(obj->normal, obj->vec_tmp) < 0.0f)
		return (-1.0f);


	obj->oc_temp->x = obj->p2->x - obj->p1->x;
	obj->oc_temp->y = obj->p2->y - obj->p1->y;
	obj->oc_temp->z = obj->p2->z - obj->p1->z;

	obj->vec_temp->x = obj->t_p->x - obj->p1->x;
	obj->vec_temp->y = obj->t_p->y - obj->p1->y;
	obj->vec_temp->z = obj->t_p->z - obj->p1->z;

	obj->vec_tmp = ft_cross_prod(obj->vec_tmp, obj->oc_temp, obj->vec_temp);
	if (ft_dot_prod(obj->normal, obj->vec_tmp) < 0.0f)
		return (-1.0f);


	obj->oc_temp->x = obj->p0->x - obj->p2->x;
	obj->oc_temp->y = obj->p0->y - obj->p2->y;
	obj->oc_temp->z = obj->p0->z - obj->p2->z;

	obj->vec_temp->x = obj->t_p->x - obj->p2->x;
	obj->vec_temp->y = obj->t_p->y - obj->p2->y;
	obj->vec_temp->z = obj->t_p->z - obj->p2->z;

	obj->vec_tmp = ft_cross_prod(obj->vec_tmp, obj->oc_temp, obj->vec_temp);
	if (ft_dot_prod(obj->normal, obj->vec_tmp) < 0.0f)
		return (-1.0f);
	return (t);
}

// bool rayTriangleIntersect( 
//     const Vec3f &orig, const Vec3f &dir, 
//     const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, 
//     float &t) 
// {
 
//     // edge 0
//     Vec3f edge0 = v1 - v0; 
//     Vec3f vp0 = P - v0; 
//     C = edge0.crossProduct(vp0); 
//     if (N.dotProduct(C) < 0) return false; // P is on the right side 
 
//     // edge 1
//     Vec3f edge1 = v2 - v1; 
//     Vec3f vp1 = P - v1; 
//     C = edge1.crossProduct(vp1); 
//     if (N.dotProduct(C) < 0)  return false; // P is on the right side 
 
//     // edge 2
//     Vec3f edge2 = v0 - v2; 
//     Vec3f vp2 = P - v2; 
//     C = edge2.crossProduct(vp2); 
//     if (N.dotProduct(C) < 0) return false; // P is on the right side; 
 
//     return true; // this ray hits the triangle 
// } 
