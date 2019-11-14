/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RTv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 11:46:52 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/18 11:46:54 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <mlx.h>
# include <math.h>
# include "./libft/get_next_line.h"
# include "./libft/libft.h"
# include <stdlib.h>

typedef struct		s_point
{
	float 			x;
	float 			y;
	float 			z;
}					t_point;

typedef struct		s_light
{
	int				type;
	float			intensity;
	t_point			*vec;
	struct s_light	*next;
}					t_light;

typedef struct		s_object
{
//	int				(*formula)(struct s_obect *object);
	t_point			*c;
	float 			h;
	float			r;
	t_point			*p1;
	t_point			*p2;
	t_point			*p3;
	int				color[3];
	struct s_object	*next;
}					t_object;

typedef struct		s_mlx
{
	t_object		*h_object;
	t_light			*h_light;
}					t_mlx;

/*
 ** tools.c
 */
void		error(char *str);
int			count_words(char **str);
void		free_splited(char **splited_str);
int			ft_isspace(char c);
float		ft_atof(char *str);

/*
 ** read_object.c
 */

void		add_lst_object(t_mlx *mlx, t_object *object);
void		read_c(t_mlx *mlx, int fd, int type);
void		read_plane(t_mlx *mlx, int fd, int type);
void		read_sphere(t_mlx *mlx, int fd, int type);
//void		get_formula(int type, t_object *object);

/*
 ** get.c
 */

void		get_color(char *str, int *r, int *g, int *b);
int 		get_type_o(char *str);
t_point		*get_point(char *str);
int 		get_type_l(char *str);
float		get_float(char *str);

/*
 ** init.c
 */

t_mlx		*init_mlx_to_zero();

#endif
