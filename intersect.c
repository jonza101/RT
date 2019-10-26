/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 16:38:57 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/26 13:03:16 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float		ft_sph_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	t_mlx *mlx = (t_mlx*)mlxx;

	mlx->oc->x = origin->x - obj->c->x;
	mlx->oc->y = origin->y - obj->c->y;
	mlx->oc->z = origin->z - obj->c->z;

	float k1 = ft_dot_prod(dir, dir);
	float k2 = 2.0f * ft_dot_prod(mlx->oc, dir);
	float k3 = ft_dot_prod(mlx->oc, mlx->oc) - obj->radius * obj->radius;

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (0.0f);

	float t1 = (float)(-k2 + sqrtf(discr)) / (float)(2.0f * (float)k1);
	float t2 = (float)(-k2 - sqrtf(discr)) / (float)(2.0f * (float)k1);

	if (t1 < t2)
		return (t1);
	return (t2);
}

float		ft_plane_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	t_mlx *mlx = (t_mlx*)mlxx;

	float denom = ft_dot_prod(obj->normal, dir);
	if (fabs(denom) > 0.000001f)
	{
		mlx->oc->x = obj->c->x - origin->x;
		mlx->oc->y = obj->c->y - origin->y;
		mlx->oc->z = obj->c->z - origin->z;

		float t = (float)ft_dot_prod(mlx->oc, obj->normal) / (float)denom;
		if (t > 0.000001f)
			return (t);
	}
	return (0.0f);
}

float		ft_cone_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	t_mlx *mlx = (t_mlx*)mlxx;

	mlx->oc->x = origin->x - obj->c->x;
	mlx->oc->y = origin->y - obj->c->y;
	mlx->oc->z = origin->z - obj->c->z;

	obj->normal = ft_vec_normalize(obj->normal);

	float k1 = ft_dot_prod(dir, dir) - (1.0f + powf(tanf(obj->radius), 2)) * powf(ft_dot_prod(dir, obj->normal), 2);
	float k2 = 2.0f * (ft_dot_prod(dir, mlx->oc) - (1.0f + powf(tanf(obj->radius), 2)) * ft_dot_prod(dir, obj->normal) * ft_dot_prod(mlx->oc, obj->normal));
	float k3 = ft_dot_prod(mlx->oc, mlx->oc) - (1.0f + powf(tanf(obj->radius), 2)) * powf(ft_dot_prod(mlx->oc, obj->normal), 2);

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (0.0f);
	
	float t1 = (float)(-k2 + sqrtf(discr)) / (float)(2.0f * k1);
	float t2 = (float)(-k2 - sqrtf(discr)) / (float)(2.0f * k1);

	if (t1 < t2)
		return (t1);
	return (t2);
}

float		ft_cylinder_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
    t_mlx *mlx = (t_mlx*)mlxx;

    mlx->oc->x = origin->x - obj->c->x;
    mlx->oc->y = origin->y - obj->c->y;
    mlx->oc->z = origin->z - obj->c->z;

    obj->normal = ft_vec_normalize(obj->normal);

    float k1 = ft_dot_prod(dir, dir) - pow(ft_dot_prod(dir, obj->normal), 2);
    float k2 = 2.0f * (ft_dot_prod(dir, mlx->oc) - (ft_dot_prod(dir, obj->normal) * ft_dot_prod(mlx->oc, obj->normal)));
    float k3 = ft_dot_prod(mlx->oc, mlx->oc) - pow(ft_dot_prod(mlx->oc, obj->normal), 2) - pow(obj->radius, 2);

    float discr = k2 * k2 - 4.0f * k1 * k3;
    if (discr < 0.0f)
        return (0.0f);

    float t1 = (float)(-k2 + sqrtf(discr)) / (float)(2.0f * k1);
    float t2 = (float)(-k2 - sqrtf(discr)) / (float)(2.0f * k1);

    if (t1 < t2)
        return (t1);
    return (t2);

}
