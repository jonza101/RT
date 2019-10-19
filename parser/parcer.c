/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 18:09:56 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/17 18:09:57 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void		add_lst_light(t_mlx *mlx, int type, float intens, t_point *point)
{
	t_light		*new;

	new = mlx->h_light;
	if (!(mlx->h_light))
	{
		mlx->h_light = (t_light *)malloc(sizeof(t_light));
		mlx->h_light->type = type;
		mlx->h_light->vec = point;
		mlx->h_light->intensity = intens;
		mlx->h_light->next = NULL;
		return ;
	}
	while (new->next)
		new = new->next;
	new->next = (t_light *)malloc(sizeof(t_light));
	new->next->type = type;
	new->next->intensity = intens;
	new->next->vec = point;
	new->next->next = NULL;
}

void		add_light(int fd, t_mlx *mlx)
{
	int		type;
	float	intensity;
	t_point	*point;
	char	*str;

	point = NULL;
	get_next_line(fd, &str);
	type = get_type_l(str);
	free(str);
	get_next_line(fd, &str);
	intensity = get_float(str);
	free(str);
	if (type != 0)
	{
		get_next_line(fd, &str);
		point = get_point(str);
		free(str);
	}
	get_next_line(fd, &str);
	if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
		error("invalid map");
	free(str);
	add_lst_light(mlx, type, intensity, point);
}

/*
void		init_p_to_zero(t_object *object)
{
	object->p1->x = 0;
	object->p1->y = 0;
	object->p1->x = 0;
	object->p2->x = 0;
	object->p2->y = 0;
	object->p2->z = 0;
	object->p3->x = 0;
	object->p3->y = 0;
	object->p3->z = 0;
}
*/

void		add_object(int fd, t_mlx *mlx)
{
	char	*str;
	int		type;

	get_next_line(fd, &str);
	type = get_type_o(str);
	free(str);
	if (type == 0)
		read_plane(mlx, fd, type);
	else if (type == 1)
		read_sphere(mlx, fd, type);
	else
		read_c(mlx, fd, type);
	get_next_line(fd, &str);
	if ((ft_strcmp(str, "}\n")) && ft_strcmp(str, "}"))
		error("invalid map");
	free(str);
}

t_mlx		*read_file(int fd)
{
	char	*str;
	t_mlx	*mlx;

	mlx = init_mlx_to_zero();
	while (get_next_line(fd, &(str)))
	{
		if (ft_strcmp(str, "light {") == 0)
			add_light(fd, mlx);
		else if (ft_strcmp(str, "object {") == 0)
			add_object(fd, mlx);
		else
			error("invalid map");
	}
	return (mlx);
}

#include <stdio.h>

int			main(int arc, char **argv)
{
	int		fd;
	t_mlx	*mlx;

	if (arc != 2)
	{
		error("usage: [map]");
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		error("wrong file");
	if(!(mlx = read_file(fd)))
		error("error");
	printf("objects: \n\n");
	while (mlx->h_object)
	{
		if (mlx->h_object->c)
			printf("centre = %f %f %f\n", mlx->h_object->c->x, mlx->h_object->c->y, mlx->h_object->c->z);
		printf("radius = %f\n", mlx->h_object->r);
		printf("height = %f\n", mlx->h_object->h);
		if (mlx->h_object->p1)
			printf("point1 = %f %f %f\n", mlx->h_object->p1->x, mlx->h_object->p1->y, mlx->h_object->p1->z);
		if (mlx->h_object->p2)
			printf("point2 = %f %f %f\n", mlx->h_object->p2->x, mlx->h_object->p2->y, mlx->h_object->p2->z);
		if (mlx->h_object->p3)
			printf("point3 = %f %f %f\n", mlx->h_object->p3->x, mlx->h_object->p3->y, mlx->h_object->p3->z);
		printf("color = %d %d %d\n", mlx->h_object->color[0], mlx->h_object->color[1], mlx->h_object->color[2]);
		mlx->h_object = mlx->h_object->next;
		printf("\n");
	}
	printf("\n\n");
	printf("lights: \n\n");
	while (mlx->h_light)
	{
		printf("type = %d\n", mlx->h_light->type);
		if (mlx->h_light->vec)
			printf("point = %f %f %f\n", mlx->h_light->vec->x, mlx->h_light->vec->y, mlx->h_light->vec->z);
		printf("intensity = %f\n\n", mlx->h_light->intensity);
		mlx->h_light = mlx->h_light->next;
	}
	//function
	return (0);
}
