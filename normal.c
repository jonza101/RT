/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 17:41:14 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/24 13:10:38 by zjeyne-l         ###   ########.fr       */
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
	float r = sqrtf((point->x - obj->c->x) * (point->x - obj->c->x) + (point->z - obj->c->z) * (point->z - obj->c->z));
	normal->x = point->x - obj->c->x;
	normal->y = r * obj->radius;
	normal->z = point->z - obj->c->z;
	normal = ft_vec_normalize(normal);
	return (normal);
}
