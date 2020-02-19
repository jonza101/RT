/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:38:47 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/02/19 21:25:30 by zjeyne-l         ###   ########.fr       */
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

#define W 1600
#define H 900
#define AR ((float)W / (float)H)

#define MAX_FPS 30

#define BACKGROUND_COLOR 0x0

#define MAX_DEPTH 1

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

#define AMBIENT_L 0
#define POINT_L 1

#define EFFECTS 5
#define NONE 0
#define CEL_SHADING 1
#define SEPIA 2
#define GRAYSCALE 3
#define BLACK_WHITE 4

#define	TXT 4
#define BUMP 4
#define RGH 4

#define GPU_STR "GPU (G)"
#define CPU_STR "CPU (G)"

#define EFFECT_NONE_STR "Effect: None (C)"
#define EFFECT_CEL_SHADING_STR "Effect: Cel Shading (C) (-/+)"
#define EFFECT_SEPIA_STR "Effect: Sepia (C)"
#define EFFECT_GRAYSCALE_STR "Effect: Grayscale (C)"
#define EFFECT_BLACK_WHITE_STR "Effect: Black & White (C) (Num 1/Num 2)"

#define NEGATIVE_OFF_STR "Negative: Off (V)"
#define NEGATIVE_ON_STR "Negative: On (V)"

#define NOISE_OFF_STR "Noise: Off (F)"
#define NOISE_ON_STR "Noise: On (F) (Num 4/Num 5)"

#define SOFT_SHADOWS_OFF_STR "Soft Shadows: Off (Z)"
#define SOFT_SHADOWS_ON_STR "Soft Shadows: On (Z) (</>)"

#define AA_0 "SSAA: Off (Q, E)"
#define AA_1 "SSAA: 2x (Q, E)"
#define AA_2 "SSAA: 4x (Q, E)"
#define AA_3 "SSAA: 16x (Q, E)"

#define BUMP_OFF "Bump Mapping: Off (B)"
#define BUMP_ON "Bump Mapping: On (B)"

#define COLORED_LIGHT_OFF_STR "Colored Light (Unstable): Off (X)"
#define COLORED_LIGHT_ON_STR "Colored Light (Unstable): On (X)"


#define EDGE_THRESHOLD_MIN 0.0312f
#define EDGE_THRESHOLD_MAX 0.125f
#define SUBPIXEL_QUALITY  0.75f


typedef	struct			s_vec2
{
	float				x;
	float				y;
}						t_vec2;

typedef struct			s_vec3
{
	float				x;
	float				y;
	float				z;
}                  		t_vec3;

typedef struct 			s_img
{
	void				*img;
	int					bpp;
	int					size_line;
	int					endian;
	int					*data;

	int					w;
	int					h;

	int					txt_idx;
}						t_img;

typedef	struct			s_light				//		0 - AMBIENT		|	1 - POINT	|	2 - DIRECTIONAL		|
{
	int					type;
	float				intensity;
	int					color;
	float				radius;

	t_vec3				*vec;
}						t_light;

typedef	struct			s_obj
{
	int					type;
	t_vec3				*c;
	float				radius;
	t_vec3				*normal;

	int					color;
	float				specular;
	float           	mirrored;
	float				transparency;
	float				refractive_index;
	t_img				*txt;
	t_img				*bump;
	t_img				*rgh;
	t_vec2				*uv;

	float				(*intersect) (t_vec3 *origin, t_vec3 *dir, struct s_obj *obj);
	t_vec3*				(*normal_calc) (t_vec3 *normal, t_vec3 *dir, t_vec3 *point, struct s_obj *obj);
	int					(*txt_mapping) (struct s_obj *obj, t_vec3 *normal, t_vec3 *point);
	t_vec3*				(*bump_mapping) (struct s_obj *obj, t_vec3 *normal, t_vec3 *point);

	t_vec3				*oc_temp;
	t_vec3				*vec_temp;
	t_vec3				*vec_tmp;
	t_vec3				*t_p;
	t_vec3				*n_temp;
	t_vec3				*tmp;

	struct	s_obj		*next;
}						t_obj;

typedef struct			s_mlx
{
	void				*mlx;
	void				*win;
	void				*img;
	int					*data;
	int					bpp;
	int					size_line;
	int					endian;

	float           	dx;
	float           	dy;

	t_vec3				*cam;
	t_vec3				*dir;
	t_vec3				*oc;

	t_vec3				*point;
	t_vec3				*normal;
	t_vec3				*light_dir;

	t_vec3				*neg_dir;
	t_vec3				*s_refl;

	t_vec3				*refl_ray;
	t_vec3				*refr_ray;

	t_vec3				*s_dir;

	float				closest;

	t_obj				*obj_head;
	t_obj				**obj;
	int					obj_count;

	t_light				**light;
	int					light_count;

	int					wsad[4];
	int					arrow[4];
	int					up_down[2];
	int					shift;

	clock_t				last_time;
	char				*render_device;
	void				(*render_func)(struct s_mlx *mlx);

	int					cel_band;

	char				*effect_str[EFFECTS];
	char				*curr_effect;
	char				effect_i;

	int					colored_light;
	char				*colored_light_str[2];

	int					soft_shadows;
	int					ss_cell;
	char				*soft_sh_str[2];

	int 				negative;
	char				*negative_str[2];

	int					noise;
	char				*noise_str[2];

	int					bw_factor;
	int					ns_factor;

	t_img				*txt[TXT];
	t_img 				*bump[BUMP];
	t_img				*rgh[RGH];

	int					bump_mapping;
	char 				*bump_str[2];

	t_vec3				*aa_dir;
	t_vec3				*aa_dir_cpy;

	int					aa_idx;
	int					aa_val[4];
	char				*aa_str[4];
	cl_double3			aa_misc;			//		|	X - PIX_LEN_X	|	Y - PIXEL_LEN_Y		|	Z - AA_VAL	|


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
	cl_float			*obj_radius;
	cl_int				*obj_color;
	cl_float			*obj_specular;
	cl_float			*obj_mirrored;
	cl_float			*obj_transparency;
	cl_float			*obj_refractive_index;
	cl_int				*obj_type;

	cl_float3			*light_vec;
	cl_int				*light_type;
	cl_float			*light_intensity;

	cl_ulong4			*obj_txt;				//		|	X - TXT_W	|	Y - TXT_H		|	Z - COLOR				|	W - TXT_OFFSET	|
	cl_int3				*obj_txt_misc;			//		|	X - TXT_IDX	|
	unsigned long		txt_pix;
	cl_ulong4			*obj_bump_map;			//		|	X - BUMP_W	|	Y - BUMP_H		|	Z - COLOR				|	W - BUMP_OFFSET	|
	cl_int				*obj_bump_idx;
	unsigned long		bump_pix;

	int					gpu;

	cl_mem				gpu_obj_pos;
	cl_mem				gpu_obj_normal;
	cl_mem				gpu_obj_radius;
	cl_mem				gpu_obj_color;
	cl_mem				gpu_obj_specular;
	cl_mem				gpu_obj_mirrored;
	cl_mem				gpu_obj_transparency;
	cl_mem				gpu_obj_refractive_index;
	cl_mem				gpu_obj_type;

	cl_mem				gpu_light_vec;
	cl_mem				gpu_light_type;
	cl_mem				gpu_light_intensity;

	cl_mem				gpu_obj_txt;
	cl_mem				gpu_obj_txt_misc;

	cl_mem				gpu_obj_bump;
	cl_mem				gpu_obj_bump_idx;
}						t_mlx;


float					ft_min(float a, float b);
float					ft_max(float a, float b);
float					ft_clamp(float a, float min, float max);

float					ft_dot_prod(t_vec3 *a, t_vec3 *b);
t_vec3					*ft_cross_prod(t_vec3 *vec, t_vec3 *a, t_vec3 *b);
float					ft_vec_len(t_vec3 *vec);
t_vec3					*ft_vec_normalize(t_vec3 *vec);
int						ft_color_lum(int color, float lum);
int						ft_sum_color(int c1, int c2, float k1, float k2);
int						ft_mix_colors(int c1, int c2);
int						ft_to_sepia(int color);
int						ft_to_grayscale(int color);
int						ft_to_negative(int color);
int						ft_to_black_white(int color, int factor);
int						ft_to_noise(int color, int factor);

void        	        ft_render(t_mlx *mlx);

void					ft_init_gpu(t_mlx *mlx);
void					ft_load_cl_files(t_mlx *mlx);
void					ft_execute_kernel(t_mlx *mlx);

t_vec3					*ft_vec_rotate(t_vec3 *vec, float dx, float dy, t_vec3 *temp);
void					ft_move(t_mlx *mlx);

float					ft_sph_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float					ft_plane_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float					ft_cone_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);
float					ft_cylinder_intersect(t_vec3 *origin, t_vec3 *dir, t_obj *obj);

t_vec3					*ft_sph_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3					*ft_plane_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3					*ft_cone_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);
t_vec3					*ft_cylinder_normal_calc(t_vec3 *normal, t_vec3 *dir, t_vec3 *point, t_obj *obj);

int						ft_sph_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);
int						ft_plane_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);
int 					ft_cylinder_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);
int						ft_cone_txt_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);

t_vec3 					*ft_sph_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);
t_vec3					*ft_plane_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);
t_vec3					*ft_cylinder_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);
t_vec3					*ft_cone_bump_map(t_obj *obj, t_vec3 *normal, t_vec3 *point);

int						ft_key_press(int keycode, t_mlx *mlx);
int						ft_key_realese(int keycode, t_mlx *mlx);

void 					ft_init_txt(t_mlx *mlx);
void 					ft_init_bump(t_mlx *mlx);
void					ft_init_rgh(t_mlx *mlx);
