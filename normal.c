/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 17:41:14 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/26 13:23:59 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3		*ft_sph_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj)
{
	normal->x = point->x - obj->c->x;
	normal->y = point->y - obj->c->y;
	normal->z = point->z - obj->c->z;
	normal = ft_vec_normalize(normal);
	return (normal);
}

t_vec3		*ft_plane_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj)
{
	normal->x = obj->normal->x;
	normal->y = obj->normal->y;
	normal->z = obj->normal->z;
	return (normal);
}

t_vec3		*ft_cone_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj)
{
    normal->x = point->x - obj->c->x;
    normal->y = point->y - obj->c->y;
    normal->z = point->z - obj->c->z;
    float side_len = sqrt(ft_dot_prod(normal, normal));
    side_len /= cos(obj->radius);
    if (ft_dot_prod(normal, obj->normal) < 0)
        side_len = -side_len;
    normal->x = obj->c->x + obj->normal->x * side_len;
    normal->y = obj->c->y + obj->normal->y * side_len;
    normal->z = obj->c->z + obj->normal->z * side_len;
    normal->x = point->x - normal->x;
    normal->y = point->y - normal->y;
    normal->z = point->z - normal->z;
    normal = ft_vec_normalize(normal);
    return (normal);
}

t_vec3		*ft_cylinder_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj)
{
    normal->x = point->x - obj->c->x;
    normal->y = point->y - obj->c->y;
    normal->z = point->z - obj->c->z;
    float dis = ft_dot_prod(normal, obj->normal);
    normal->x = obj->c->x + obj->normal->x * dis;
    normal->y = obj->c->y + obj->normal->y * dis;
    normal->z = obj->c->z + obj->normal->z * dis;
    normal->x = point->x - normal->x;
    normal->y = point->y - normal->y;
    normal->z = point->z - normal->z;
    normal = ft_vec_normalize(normal);
    return (normal);
}