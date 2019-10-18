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

int			ft_isspace(char c)
{
	if (c == 32)
		return (1);
	return (0);
}

float		ft_atof(char *str)
{
	float	res;
	float	power;
	size_t	i;
	float		sign;

	res = 0.0;
	i = 0;
	power = 1.0;
	while (ft_isspace(str[i]))
		i++;
	sign = (str[i] == '-') ? -1 : 1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	i--;
	while (ft_isdigit(str[++i]))
		res = res * 10 + (str[i] - '0');
	if (str[i] == '.')
		i++;
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + (str[i] - '0');
		power *= 10.0;
		i++;
	}
	return (res * sign / power);
}

void		error(char *str)
{
	ft_putstr(str);
	ft_putchar('\n');
	exit(1);
}

int		count_words(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

t_mlx		*init_mlx_to_zero()
{
	t_mlx	*mlx;

	if (!(mlx = (t_mlx *)malloc(sizeof(t_mlx))))
		error("malloc error");
	mlx->h_light = NULL;
	mlx->h_object = NULL;
	return (mlx);
}

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
void		add_lst_object(t_mlx *mlx, t_object *object)
{
	t_object	*new;

	new = mlx->h_object;
	if(!(mlx->h_object))
	{
		mlx->h_object = object;
		return ;
	}
	while (new->next)
		new = new->next;
	new->next = object;
}

void	free_splited(char **splited_str)
{
	int		i;

	i = 0;
	while (splited_str[i])
	{
		ft_strdel(&splited_str[i]);
		i++;
	}
	free(splited_str);
}

int 		get_type_l(char *str)
{
	int 	type;
	char 	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
		error("invalid map");
	if (!(ft_strcmp(splited_str[2], "ambient")))
		type = 0;
	else if (!(ft_strcmp(splited_str[2], "point")))
		type = 1;
	else if (!(ft_strcmp(splited_str[2], "directional")))
		type = 2;
	else
		error("invalid type of light");
	free_splited(splited_str);
	return (type);
}

float		get_float(char *str)
{
	float	intensity;
	char	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
		error("invalid map");
	intensity = atof(splited_str[2]);
	free_splited(splited_str);
	return (intensity);
}

t_point		*get_point(char *str)
{
	t_point	*point;
	char	**splited_str;

	point = (t_point *)malloc(sizeof(t_point));
	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 5)
		error("invalid map");
	point->x = ft_atof(splited_str[2]);
	point->y = ft_atof(splited_str[3]);
	point->z = ft_atof(splited_str[4]);
	free_splited(splited_str);
	return (point);
}

void		add_light(int fd, t_mlx *mlx)
{
	int 	type;
	float	intensity;
	t_point	*point;
	char 	*str;

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

int 		get_type_o(char *str) {
	int type;
	char **splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
		error("invalid map");
	if (!(ft_strcmp(splited_str[2], "plane")))
		type = 0;
	else if (!(ft_strcmp(splited_str[2], "sphere")))
		type = 1;
	else if (!(ft_strcmp(splited_str[2], "cylinder")))
		type = 2;
	else if (!(ft_strcmp(splited_str[2], "cone")))
		type = 3;
	else
		error("invalid type of light");
	free_splited(splited_str);
	return (type);
}

void		get_color(char *str, int *r, int *g, int *b)
{
	char	**splited_str;

	if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 5)
		error("invalid map");
	*r = ft_atoi(splited_str[2]);
	*g = ft_atoi(splited_str[3]);
	*b = ft_atoi(splited_str[4]);
	free_splited(splited_str);
}

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

void		read_sphere(t_mlx *mlx, int fd, int type)
{
	char	*str;
	t_object	*object;

	object = (t_object *)malloc(sizeof(t_object));
	get_next_line(fd, &str);
	object->c = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->r = get_float(str);
	free(str);
	get_next_line(fd, &str);
	get_color(str, &(object->color[0]), &(object->color[1]), &(object->color[2]));
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

void		read_plane(t_mlx *mlx, int fd, int type)
{
	char	*str;
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
	get_color(str, &(object->color[0]), &(object->color[1]), &(object->color[2]));
	free(str);
	object->next = NULL;
	//get_formula(type, object);
	object->c = NULL;
	object->h = 0;
	object->r = 0;
	add_lst_object(mlx, object);
}

void		read_c(t_mlx *mlx, int fd, int type)
{
	char	*str;
	t_object	*object;

	object = (t_object *)malloc(sizeof(t_object));
	get_next_line(fd, &str);
	object->c = get_point(str);
	free(str);
	get_next_line(fd, &str);
	object->r = get_float(str);
	free(str);
	get_next_line(fd, &str);
	object->h = get_float(str);
	free(str);
	get_next_line(fd, &str);
	get_color(str, &(object->color[0]), &(object->color[1]), &(object->color[2]));
	free(str);
	object->next = NULL;
	//get_formula(type, object);
	object->p1 = NULL;
	object->p2 = NULL;
	object->p3 = NULL;
	add_lst_object(mlx, object);
}

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
	char 	*str;
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