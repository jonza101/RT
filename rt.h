/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:38:47 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/29 00:56:04 by zjeyne-l         ###   ########.fr       */
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
#define THREAD 16

#define W 1920
#define H 1080
#define AR ((double)W / (double)H)

#define BACKGROUND_COLOR 0x0

#define CAM_SPEED 1.5f
#define CAM_VERT_SPEED 0.25f
#define HORIZONTAL_SENS 0.05f
#define VERTICAL_SENS 0.05f

typedef struct		s_vec3
{
	double			x;
	double			y;
	double			z;
}                   t_vec3;

typedef	struct		s_light				//		0 - AMBIENT		|	1 - POINT	|	2 - DIRECTIONAL		|
{
	int				type;
	double			intensity;
	t_vec3			*vec;
}					t_light;

typedef	struct		s_obj
{
	t_vec3			*c;
	double			radius;
	double			height;
	t_vec3			*normal;

	int				color;
	double			specular;

	double			(*intersect) (void* mlxx, t_vec3 *origin, t_vec3 *dir, struct s_obj *obj);
	t_vec3*			(*normal_calc) (t_vec3 *normal, t_vec3 *dir, t_vec3 *point, struct s_obj *obj);

	double           mirrored;
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

	double           dx;
	double           dy;

	t_vec3			*cam;
	t_vec3			*dir;

	t_vec3			*oc;

	t_vec3			*point;
	t_vec3			*normal;
	t_vec3			*light_dir;

	t_vec3			*neg_dir;
	t_vec3			*s_refl;

	double			closest;

	double			t1;
	double			t2;

	t_obj			**obj;
	int				obj_count;

	t_light			**light;
	int				light_count;

	int				thread_i;
	struct	s_mlx *mlx_thread[THREAD];

	int				wsad[4];
	int				arrow[4];
	int				up_down[2];
}					t_mlx;


double				ft_max(double a, double b);

double				ft_dot_prod(t_vec3 *a, t_vec3 *b);
double				ft_vec_len(t_vec3 *vec);
t_vec3				*ft_vec_normalize(t_vec3 *vec);
int					ft_color_convert(int color, double lum);
int         ft_sum_color(int c1, int c2, double k1, double k2);

void                ft_render(t_mlx *mlx);

void				ft_thread_init(t_mlx *mlx);
void				ft_thread(t_mlx *mlx);

void				ft_move(t_mlx *mlx);

double				ft_sph_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);
double				ft_plane_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);
double				ft_cone_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);
double				ft_cylinder_intersect(void *mlxx, t_vec3 *origin, t_vec3 *dir, t_obj *obj);

t_vec3				*ft_sph_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3				*ft_plane_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3				*ft_cone_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3				*ft_cylinder_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);

int						ft_key_press(int keycode, t_mlx *mlx);
int						ft_key_realese(int keycode, t_mlx *mlx);
