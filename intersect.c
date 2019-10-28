/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 16:38:57 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/28 23:57:07 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double		ft_sph_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	t_mlx *mlx = (t_mlx*)mlxx;

	mlx->oc->x = origin->x - obj->c->x;
	mlx->oc->y = origin->y - obj->c->y;
	mlx->oc->z = origin->z - obj->c->z;

	double k1 = ft_dot_prod(dir, dir);
	double k2 = 2.0f * ft_dot_prod(mlx->oc, dir);
	double k3 = ft_dot_prod(mlx->oc, mlx->oc) - obj->radius * obj->radius;

	double discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (0.0f);

	double t1 = (double)(-k2 + sqrtf(discr)) / (double)(2.0f * (double)k1);
	double t2 = (double)(-k2 - sqrtf(discr)) / (double)(2.0f * (double)k1);

	if (t1 < t2)
		return (t1);
	return (t2);
}

double		ft_plane_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	t_mlx *mlx = (t_mlx*)mlxx;

	double denom = ft_dot_prod(obj->normal, dir);
	if (fabs(denom) > 0.000001f)
	{
		mlx->oc->x = obj->c->x - origin->x;
		mlx->oc->y = obj->c->y - origin->y;
		mlx->oc->z = obj->c->z - origin->z;

		double t = (double)ft_dot_prod(mlx->oc, obj->normal) / (double)(denom);
		if (t > 0.000001f)
			return (t);
	}
	return (0.0f);
}

double		ft_cone_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
	t_mlx *mlx = (t_mlx*)mlxx;

	mlx->oc->x = origin->x - obj->c->x;
	mlx->oc->y = origin->y - obj->c->y;
	mlx->oc->z = origin->z - obj->c->z;

	obj->normal = ft_vec_normalize(obj->normal);

	double k1 = ft_dot_prod(dir, dir) - (1.0f + powf(tanf(obj->radius), 2)) * powf(ft_dot_prod(dir, obj->normal), 2);
	double k2 = 2.0f * (ft_dot_prod(dir, mlx->oc) - (1.0f + powf(tanf(obj->radius), 2)) * ft_dot_prod(dir, obj->normal) * ft_dot_prod(mlx->oc, obj->normal));
	double k3 = ft_dot_prod(mlx->oc, mlx->oc) - (1.0f + powf(tanf(obj->radius), 2)) * powf(ft_dot_prod(mlx->oc, obj->normal), 2);

	double discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (0.0f);
	
	double t1 = (double)(-k2 + sqrtf(discr)) / (double)(2.0f * k1);
	double t2 = (double)(-k2 - sqrtf(discr)) / (double)(2.0f * k1);

	if (t1 < t2)
		return (t1);
	return (t2);
}

double		ft_cylinder_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj)
{
    t_mlx *mlx = (t_mlx*)mlxx;

    mlx->oc->x = origin->x - obj->c->x;
    mlx->oc->y = origin->y - obj->c->y;
    mlx->oc->z = origin->z - obj->c->z;

    obj->normal = ft_vec_normalize(obj->normal);

    double k1 = ft_dot_prod(dir, dir) - pow(ft_dot_prod(dir, obj->normal), 2);
    double k2 = 2.0f * (ft_dot_prod(dir, mlx->oc) - (ft_dot_prod(dir, obj->normal) * ft_dot_prod(mlx->oc, obj->normal)));
    double k3 = ft_dot_prod(mlx->oc, mlx->oc) - pow(ft_dot_prod(mlx->oc, obj->normal), 2) - pow(obj->radius, 2);

    double discr = k2 * k2 - 4.0f * k1 * k3;
    if (discr < 0.0f)
        return (0.0f);

    double t1 = (double)(-k2 + sqrtf(discr)) / (double)(2.0f * k1);
    double t2 = (double)(-k2 - sqrtf(discr)) / (double)(2.0f * k1);

    if (t1 < t2)
        return (t1);
    return (t2);

}
