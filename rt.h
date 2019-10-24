/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:38:47 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/24 13:11:02 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>

#include "get_next_line.h"
#include "mac_keys.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include <pthread.h>

#define W 1920
#define H 1080
#define AR ((float)W / (float)H)

#define BACKGROUND_COLOR 0x0

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

typedef	struct		s_obj
{
	t_vec3			*c;
	float			radius;
	float			height;
	t_vec3			*normal;

	int				color;
	float			specular;

	float			(*intersect) (void* mlxx, t_vec3 *origin, t_vec3 *dir, struct s_obj *obj);
	t_vec3*			(*normal_calc) (t_vec3 *normal, t_vec3 *point, struct s_obj *obj);
}					t_obj;

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

	t_vec3			*point;
	t_vec3			*normal;
	t_vec3			*light_dir;

	t_vec3			*neg_dir;
	t_vec3			*s_refl;

	float			closest;

	float			t1;
	float			t2;

	t_obj			**obj;
	int				obj_count;

	t_light			**light;
	int				light_count;
}					t_mlx;


float				ft_max(float a, float b);

float				ft_dot_prod(t_vec3 *a, t_vec3 *b);
float				ft_vec_len(t_vec3 *vec);
t_vec3				*ft_vec_normalize(t_vec3 *vec);
int					ft_color_convert(int color, double lum);

void                ft_render(t_mlx *mlx);

float				ft_sph_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float				ft_plane_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float				ft_cone_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);

t_vec3				*ft_sph_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj);
t_vec3				*ft_plane_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj);
t_vec3				*ft_cone_normal_calc(t_vec3 *normal, t_vec3 *point, t_obj *obj);
