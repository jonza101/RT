/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:27:08 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/19 13:27:09 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void			add_lst_object(t_mlx *mlx, t_object *object)
{
	t_object	*new;

	new = mlx->h_object;
	if (!(mlx->h_object))
	{
		mlx->h_object = object;
		return ;
	}
	while (new->next)
		new = new->next;
	new->next = object;
}

void			read_c(t_mlx *mlx, int fd, int type)
{
	char		*str;
	t_object	*object;

	object = (t_object *)malloc(sizeof(t_object));
	get_next_line(fd, &str);
	object->p1 = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->p2 = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->r = get_float(str);
	free(str);
	get_next_line(fd, &str);
	get_color(str, &(object->color[0]),
			&(object->color[1]), &(object->color[2]));
	free(str);
	object->next = NULL;
	//get_formula(type, object);
	object->c = NULL;
	object->h = 0;
	object->p3 = NULL;
	add_lst_object(mlx, object);
}

void			read_plane(t_mlx *mlx, int fd, int type)
{
	char		*str;
	t_object	*object;

	object = (t_object *)malloc(sizeof(t_object));
	get_next_line(fd, &str);
	object->p1 = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->p2 = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->p3 = get_point(str);
	free(str);
	get_next_line(fd, &str);
	get_color(str, &(object->color[0]),
			&(object->color[1]), &(object->color[2]));
	free(str);
	object->next = NULL;
	//get_formula(type, object);
	object->c = NULL;
	object->h = 0;
	object->r = 0;
	add_lst_object(mlx, object);
}

void			read_sphere(t_mlx *mlx, int fd, int type)
{
	char		*str;
	t_object	*object;

	object = (t_object *)malloc(sizeof(t_object));
	get_next_line(fd, &str);
	object->c = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->r = get_float(str);
	free(str);
	get_next_line(fd, &str);
	get_color(str, &(object->color[0]),
			&(object->color[1]), &(object->color[2]));
	free(str);
	object->h = 0;
	object->next = NULL;
	//get_formula(type, object);
	object->p1 = NULL;
	object->p2 = NULL;
	object->p3 = NULL;
//	init_p_to_zero(object);
	add_lst_object(mlx, object);
}

/*
void		get_formula(int type, t_object *object)
{
	if (type == 0)
		object->formula = &plane_formula;
	else if (type == 1)
		object->formula = &sphere_formula;
	else if (type == 2)
		object->formula = &cylinder_formula;
	else
		object->formula = &cone_formula;
}
*/
