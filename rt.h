/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:38:47 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/11/10 19:13:50 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>

#include "get_next_line.h"
#include "mac_keys.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define W 1920
#define H 1080
#define AR ((float)W / (float)H)

#define MAX_FPS 30

#define CL_OBJ_STR 14
#define CL_LIGHT_STR 15

#define BACKGROUND_COLOR 0x0

#define CAM_SPEED 0.5f
#define CAM_ACC_SPEED 1.75f
#define CAM_VERT_SPEED 0.25f
#define HORIZONTAL_SENS 0.05f
#define VERTICAL_SENS 0.05f

#define MIN_VERT_CAM_VAL -1.57f
#define MAX_VERT_CAM_VAL 1.57f

#define SPHERE 0
#define PLANE 1
#define CONE 2
#define CYLINDER 3
#define TRIANGLE 4

#define AMBIENT_L 0
#define POINT_L 1

typedef struct		s_vec3
{
	float			x;
	float			y;
	float			z;
}                  		 t_vec3;

typedef	struct		s_light				//		0 - AMBIENT		|	1 - POINT	|	2 - DIRECTIONAL		|
{
	int				type;
	float			intensity;
	t_vec3			*vec;
}					t_light;

typedef	struct		s_obj
{
	int					type;
	t_vec3			*c;
	float			radius;
	t_vec3			*normal;

	t_vec3			*p0;
	t_vec3			*p1;
	t_vec3			*p2;

	int					color;
	float			specular;
	float           mirrored;

	float			(*intersect) (t_vec3 *origin, t_vec3 *dir, struct s_obj *obj);
	t_vec3*			(*normal_calc) (t_vec3 *normal, t_vec3 *dir, t_vec3 *point, struct s_obj *obj);

	t_vec3			*oc_temp;
	t_vec3			*vec_temp;
	t_vec3			*vec_tmp;
	t_vec3			*t_p;

	struct	s_obj	*next;
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

	float           dx;
	float           dy;

	t_vec3			*cam;
	t_vec3			*dir;

	t_vec3			*oc;

	t_vec3			*point;
	t_vec3			*normal;
	t_vec3			*light_dir;

	t_vec3			*neg_dir;
	t_vec3			*s_refl;

	t_vec3			*refl_ray;

	float			closest;

	t_obj			*obj_head;
	t_obj			**obj;
	int				obj_count;

	t_light			**light;
	int				light_count;

	int				wsad[4];
	int				arrow[4];
	int				up_down[2];
	int				shift;

	clock_t			last_time;
	char			*render_device;
	void			(*render_func)(struct s_mlx *mlx);

	cl_int				ret;
	cl_platform_id		platform_id;
	cl_uint				ret_num_platforms;
	cl_device_id		device_id;
	cl_uint				ret_num_devices;
	cl_context			contex;	
	cl_command_queue	command_queue;
	cl_program			program;
	cl_kernel			kernel;

	cl_mem				buffer;
	size_t				global_work_size;
	size_t				local_work_size;

	cl_float3			*obj_pos;
	cl_float3			*obj_normal;
	float				*obj_radius;
	int					*obj_color;
	float				*obj_specular;
	float				*obj_mirrored;
	int					*obj_type;

	cl_float3			*light_vec;
	int					*light_type;
	float				*light_intensity;

	int						gpu;

	cl_mem				gpu_obj_pos;
	cl_mem				gpu_obj_normal;
	cl_mem				gpu_obj_radius;
	cl_mem				gpu_obj_color;
	cl_mem				gpu_obj_specular;
	cl_mem				gpu_obj_mirrored;
	cl_mem				gpu_obj_type;

	cl_mem				gpu_light_vec;
	cl_mem				gpu_light_type;
	cl_mem				gpu_light_intensity;

	cl_mem				gpu_scene;
}					t_mlx;


float				ft_clamp(float a, float min, float max);

float				ft_dot_prod(t_vec3 *a, t_vec3 *b);
t_vec3				*ft_cross_prod(t_vec3 *vec, t_vec3 *a, t_vec3 *b);
float				ft_vec_len(t_vec3 *vec);
t_vec3				*ft_vec_normalize(t_vec3 *vec);
int					ft_color_convert(int color, float lum);
int        			 ft_sum_color(int c1, int c2, float k1, float k2);

void                ft_render(t_mlx *mlx);

void				ft_init_gpu(t_mlx *mlx);
void				ft_load_cl_files(t_mlx *mlx);
void				ft_execute_kernel(t_mlx *mlx);

t_vec3				*ft_vec_rotate(t_vec3 *vec, float dx, float dy, t_vec3 *temp);
void				ft_move(t_mlx *mlx);

float				ft_sph_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float				ft_plane_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float				ft_cone_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float				ft_cylinder_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float				ft_triangle_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);

t_vec3				*ft_sph_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3				*ft_plane_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3				*ft_cone_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3				*ft_cylinder_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3      		*ft_triangle_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);

int					ft_key_press(int keycode, t_mlx *mlx);
int					ft_key_realese(int keycode, t_mlx *mlx);
