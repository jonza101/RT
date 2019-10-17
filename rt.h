/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:38:47 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/17 16:55:07 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>

#include "get_next_line.h"
#include "mac_keys.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <pthread.h>

#define W 1920
#define H 1080
#define AR ((float)W / (float)H)

typedef struct		s_vec3
{
	float			x;
	float			y;
	float			z;
}                   t_vec3;

typedef	struct		s_light				//		0 - AMBIENT		|	1 - POINT	|	2 - DIRECTIONAL		|
{
	int				type;
	float			intensity;
	t_vec3			*vec;
}					t_light;

typedef struct		s_sphere
{
	t_vec3			*center;
	float			radius;
	int				color;
}					t_sphere;

typedef struct		s_mlx
{
	void			*mlx;
	void			*win;
	void			*img;
	int				*data;
	int				bpp;
	int				size_line;
	int				endian;

	t_vec3			*cam;
	t_vec3			*dir;

	t_vec3			*oc;

	t_vec3			*l_point;
	t_vec3			*l_normal;
	t_vec3			*l_vec;

	float			t1;
	float			t2;

	t_sphere		**sph;

	t_light			**light;
}					t_mlx;


float				ft_dot_prod(t_vec3 *a, t_vec3 *b);
float				ft_vec_len(t_vec3 *vec);
int					ft_color_convert(int color, double lum);

void                ft_render(t_mlx *mlx);
