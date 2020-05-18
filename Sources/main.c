/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:42:38 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/05/19 00:08:43 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_close(int i)
{
	i = 1;
	exit(0);
}

int		ft_gameloop(t_mlx *mlx)
{
	clock_t start_time = clock();

	mlx->delta_time =  ((double)(start_time - mlx->last_time) / CLOCKS_PER_SEC);
	int fps = (double)1.0f / (double)mlx->delta_time;
	char *fps_str = ft_itoa(fps);

	if (fps <= MAX_FPS)
	{
		ft_move(mlx);

		// ft_render(mlx);
		mlx->render_func(mlx);

		mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);

		mlx_string_put(mlx->mlx, mlx->win, 10, 20, 0xFFFFFF, fps_str);
		mlx_string_put(mlx->mlx, mlx->win, 10, 40, 0xFFFFFF, mlx->render_device);
		mlx_string_put(mlx->mlx, mlx->win, 10, 60, 0xFFFFFF, mlx->curr_effect);
		mlx_string_put(mlx->mlx, mlx->win, 10, 80, 0xFFFFFF, mlx->negative_str[mlx->negative]);
		mlx_string_put(mlx->mlx, mlx->win, 10, 100, 0xFFFFFF, mlx->noise_str[mlx->noise]);
		mlx_string_put(mlx->mlx, mlx->win, 10, 120, 0xFFFFFF, mlx->soft_sh_str[mlx->soft_shadows]);
		mlx_string_put(mlx->mlx, mlx->win, 10, 140, 0xFFFFFF, mlx->aa_str[mlx->aa_idx]);
		mlx_string_put(mlx->mlx, mlx->win, 10, 160, 0xFFFFFF, mlx->norm_str[mlx->norm_mapping]);
		mlx_string_put(mlx->mlx, mlx->win, 10, 180, 0xFFFFFF, mlx->colored_light_str[mlx->colored_light]);

		mlx->last_time = start_time;
	}
	free(fps_str);

    return (0);
}

void	ft_init(t_mlx *mlx)
{
	srand(time(NULL));

	ft_init_txt(mlx);
	ft_init_norm(mlx);
	ft_init_rgh(mlx);

	mlx->cam = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->cam->x = 0.0f;
    mlx->cam->y = 0.0f;
    mlx->cam->z = -0.5f;

    mlx->dx = 0.0f;
    mlx->dy = 0.0f;

	mlx->oc = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->dir = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->point = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->normal = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light_dir = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->neg_dir = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->s_refl = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->refl_ray = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->refr_ray = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->s_dir = (t_vec3*)malloc(sizeof(t_vec3));


	mlx->obj_count = 8;
	mlx->obj = (t_obj**)malloc(sizeof(t_obj*) * mlx->obj_count);

	int i = -1;
	while (++i < mlx->obj_count)
	{
		mlx->obj[i] = (t_obj*)malloc(sizeof(t_obj));
		mlx->obj[i]->c = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->normal = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->oc_temp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->vec_temp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->vec_tmp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->t_p = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->n_temp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->tmp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->uv = (t_vec2*)malloc(sizeof(t_vec2));

		mlx->obj[i]->type = -1;

		mlx->obj[i]->color = 0x0;
		mlx->obj[i]->radius = 0.0f;
		mlx->obj[i]->specular = 0.0f;
		mlx->obj[i]->mirrored = 0.0f;
		mlx->obj[i]->transparency = 0.0f;
		mlx->obj[i]->refractive_index = 1.0f;
		mlx->obj[i]->txt = NULL;
		mlx->obj[i]->norm = NULL;
		mlx->obj[i]->rgh = NULL;
	}
	mlx->obj_count = 3;

	mlx->obj[0]->type = PLANE;
	mlx->obj[0]->c->x = 0.0f;
	mlx->obj[0]->c->y = -1.5f;
	mlx->obj[0]->c->z = 25.0f;
	mlx->obj[0]->normal->x = 0.0f;
	mlx->obj[0]->normal->y = 1.0f;
	mlx->obj[0]->normal->z = 0.0f;
	mlx->obj[0]->color = 0xFFFFFF;
	mlx->obj[0]->specular = 32.0f;
	mlx->obj[0]->txt = mlx->txt[0];
	mlx->obj[0]->norm = mlx->norm[0];
	mlx->obj[0]->rgh = mlx->rgh[0];
	mlx->obj[0]->mirrored = 0.25f;
	mlx->obj[0]->intersect = ft_plane_intersect;
	mlx->obj[0]->normal_calc = ft_plane_normal_calc;
	mlx->obj[0]->txt_mapping = ft_plane_txt_map;
	mlx->obj[0]->norm_mapping = ft_plane_norm_map;
	mlx->obj[0]->rgh_mapping = ft_plane_rgh_map;

	mlx->obj[1]->type = SPHERE;
	mlx->obj[1]->c->x = 0.0f;
	mlx->obj[1]->c->y = -0.75f;
	mlx->obj[1]->c->z = 5.0f;
	mlx->obj[1]->radius = 0.75f;
	mlx->obj[1]->normal->x = 0.0f;
	mlx->obj[1]->normal->y = 1.0f;
	mlx->obj[1]->normal->z = 0.0f;
	mlx->obj[1]->color = 0xBDE300;
	mlx->obj[1]->specular = 128.0f;
	mlx->obj[1]->txt = mlx->txt[3];
	mlx->obj[1]->norm = mlx->norm[3];
	mlx->obj[1]->rgh = mlx->rgh[3];
	// mlx->obj[1]->mirrored = 0.25f;
	mlx->obj[1]->intersect = ft_sph_intersect;
	mlx->obj[1]->normal_calc = ft_sph_normal_calc;
	mlx->obj[1]->txt_mapping = ft_sph_txt_map;
	mlx->obj[1]->norm_mapping = ft_sph_norm_map;
	mlx->obj[1]->rgh_mapping = ft_sph_rgh_map;

	mlx->obj[2]->type = CYLINDER;
	mlx->obj[2]->c->x = -2.5f;
	mlx->obj[2]->c->y = 0.0f;
	mlx->obj[2]->c->z = 9.0f;
	mlx->obj[2]->radius = 0.6f;
	mlx->obj[2]->normal->x = 0.15f;
	mlx->obj[2]->normal->y = 1.0f;
	mlx->obj[2]->normal->z = 0.25f;
	mlx->obj[2]->normal = ft_vec_normalize(mlx->obj[2]->normal);
	mlx->obj[2]->color = 0xBDE300;
	mlx->obj[2]->txt = mlx->txt[1];
	mlx->obj[2]->norm = mlx->norm[1];
	mlx->obj[2]->intersect = ft_cylinder_intersect;
	mlx->obj[2]->normal_calc = ft_cylinder_normal_calc;
	mlx->obj[2]->txt_mapping = ft_cylinder_txt_map;
	mlx->obj[2]->norm_mapping = ft_cylinder_norm_map;
	mlx->obj[2]->rgh_mapping = ft_cylinder_rgh_map;



	// mlx->obj[0]->type = SPHERE;
	// mlx->obj[0]->c->x = 0.15f;
	// mlx->obj[0]->c->y = 0.15f;
	// mlx->obj[0]->c->z = 5.0f;
	// mlx->obj[0]->radius = 0.5f;
	// mlx->obj[0]->color = 0x008cff;
	// mlx->obj[0]->transparency = 1.0f;
	// mlx->obj[0]->refractive_index = 1.1f;
	// mlx->obj[0]->intersect = ft_sph_intersect;
	// mlx->obj[0]->normal_calc = ft_sph_normal_calc;

	// mlx->obj[1]->type = SPHERE;
	// mlx->obj[1]->c->x = -1.5f;
	// mlx->obj[1]->c->y = 1.0f;
	// mlx->obj[1]->c->z = 5.5f;
	// mlx->obj[1]->radius = 0.75f;
	// mlx->obj[1]->color = 0x00CC99;
	// mlx->obj[1]->specular = 75.0f;
	// mlx->obj[1]->mirrored = 0.4f;
	// mlx->obj[1]->intersect = ft_sph_intersect;
	// mlx->obj[1]->normal_calc = ft_sph_normal_calc;

	// mlx->obj[2]->type = SPHERE;
	// mlx->obj[2]->c->x = -1.0f;
	// mlx->obj[2]->c->y = -1.0f;
	// mlx->obj[2]->c->z = 5.75f;
	// mlx->obj[2]->radius = 1.25f;
	// mlx->obj[2]->color = 0xBD0052;
	// mlx->obj[2]->specular = 500.0f;
	// mlx->obj[2]->mirrored = 0.5f;
	// mlx->obj[2]->intersect = ft_sph_intersect;
	// mlx->obj[2]->normal_calc = ft_sph_normal_calc;

	// mlx->obj[3]->type = PLANE;
	// mlx->obj[3]->c->x = 0.0f;
	// mlx->obj[3]->c->y = -1.25f;
	// mlx->obj[3]->c->z = 25.0f;
	// mlx->obj[3]->normal->x = 0.0f;
	// mlx->obj[3]->normal->y = 1.0f;
	// mlx->obj[3]->normal->z = 0.0f;
	// mlx->obj[3]->color = 0xFFFFFF;
	// mlx->obj[3]->mirrored = 0.75f;
	// mlx->obj[3]->intersect = ft_plane_intersect;
	// mlx->obj[3]->normal_calc = ft_plane_normal_calc;

	// mlx->obj[4]->type = CONE;
	// mlx->obj[4]->c->x = 3.5f;
	// mlx->obj[4]->c->y = 1.0f;
	// mlx->obj[4]->c->z = 7.5f;
	// mlx->obj[4]->radius = 0.25f;
	// mlx->obj[4]->normal->x = 0.5f;
	// mlx->obj[4]->normal->y = 1.0f;
	// mlx->obj[4]->normal->z = 0.75f;
	// mlx->obj[4]->normal = ft_vec_normalize(mlx->obj[4]->normal);
	// mlx->obj[4]->color = 0xBDE300;
	// mlx->obj[4]->specular = 750.0f;
	// mlx->obj[4]->mirrored = 0.1f;
	// mlx->obj[4]->intersect = ft_cone_intersect;
	// mlx->obj[4]->normal_calc = ft_cone_normal_calc;

	// mlx->obj[5]->type = CYLINDER;
    // mlx->obj[5]->c->x = -7.0f;
    // mlx->obj[5]->c->y = 0.0f;
    // mlx->obj[5]->c->z = 15.0f;
    // mlx->obj[5]->radius = 0.75f;
    // mlx->obj[5]->normal->x = -0.2f;
    // mlx->obj[5]->normal->y = 1.0f;
    // mlx->obj[5]->normal->z = 0.0f;
	// mlx->obj[5]->normal = ft_vec_normalize(mlx->obj[5]->normal);
    // mlx->obj[5]->color = 0xE85127;
    // mlx->obj[5]->specular = 750.0f;
    // mlx->obj[5]->intersect = ft_cylinder_intersect;
    // mlx->obj[5]->normal_calc = ft_cylinder_normal_calc;

	// mlx->obj[6]->type = PLANE;
	// mlx->obj[6]->c->x = 0.0f;
	// mlx->obj[6]->c->y = 0.0f;
	// mlx->obj[6]->c->z = 50.0f;
	// mlx->obj[6]->normal->x = 0.0f;
	// mlx->obj[6]->normal->y = 0.0f;
	// mlx->obj[6]->normal->z = 1.0f;
	// mlx->obj[6]->color = 0x8D41D9;
	// mlx->obj[6]->intersect = ft_plane_intersect;
	// mlx->obj[6]->normal_calc = ft_plane_normal_calc;


	mlx->light_count = 5;
	mlx->light = (t_light**)malloc(sizeof(t_light*) * mlx->light_count);
	i = -1;
	while (++i < mlx->light_count)
	{
		mlx->light[i] = (t_light*)malloc(sizeof(t_light));
		mlx->light[i]->vec = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->light[i]->intensity = 0.0f;
		mlx->light[i]->radius = 1.0f;
	}
	mlx->light_count = 1;

	mlx->light[0]->type = POINT_L;
	mlx->light[0]->vec->x = 0.0f;//-2.0f;
	mlx->light[0]->vec->y = 0.5f;
	mlx->light[0]->vec->z = 7.0f;
	mlx->light[0]->intensity = 0.75f;
	mlx->light[0]->color = 0xFF0000;

	mlx->light[1]->type = POINT_L;
	mlx->light[1]->vec->x = 2.0f;
	mlx->light[1]->vec->y = 0.5f;
	mlx->light[1]->vec->z = 7.0f;
	mlx->light[1]->intensity = 0.75f;
	mlx->light[1]->color = 0x00FF00;

	// mlx->light[2]->type = POINT_L;
	// mlx->light[2]->vec->x = -2.0f;
	// mlx->light[2]->vec->y = 0.5f;
	// mlx->light[2]->vec->z = 3.0f;
	// mlx->light[2]->intensity = 0.75f;
	// mlx->light[2]->color = 0x0000FF;

	// mlx->light[3]->type = POINT_L;
	// mlx->light[3]->vec->x = 2.0f;
	// mlx->light[3]->vec->y = 0.5f;
	// mlx->light[3]->vec->z = 3.0f;
	// mlx->light[3]->intensity = 0.75f;
	// mlx->light[3]->color = 0xFFFFF;


	// mlx->light[0]->type = POINT_L;
	// mlx->light[0]->intensity = 0.45f;
	// mlx->light[0]->color = 0xFF0000;
	// mlx->light[0]->vec->x = 0.0f;
	// mlx->light[0]->vec->y = 3.0f;
	// mlx->light[0]->vec->z = 0.0f;

	// mlx->light[1]->type = POINT_L;
	// mlx->light[1]->intensity = 0.25f;
	// mlx->light[1]->color = 0xFFFFF;
	// mlx->light[1]->vec->x = -3.25f;
	// mlx->light[1]->vec->y = 2.5f;
	// mlx->light[1]->vec->z = 4.0f;

	// mlx->light[2]->type = POINT_L;
	// mlx->light[2]->intensity = 0.85f;
	// mlx->light[2]->color = 0x0000FF;
	// mlx->light[2]->vec->x = 0.0f;
	// mlx->light[2]->vec->y = 7.5f;
	// mlx->light[2]->vec->z = 10.0f;


	i = -1;
	while (++i < 4)
	{
		mlx->wsad[i] = 0;
		mlx->arrow[i] = 0;
	}
	mlx->shift = 0;
	mlx->delta_time = 0.0f;
	mlx->last_time = 0.0f;
	mlx->gpu = 1;
	mlx->render_func = ft_execute_kernel;
	mlx->render_device = GPU_STR;

	mlx->cel_band = 1;

	mlx->effect_str[0] = EFFECT_NONE_STR;
	mlx->effect_str[1] = EFFECT_CEL_SHADING_STR;
	mlx->effect_str[2] = EFFECT_SEPIA_STR;
	mlx->effect_str[3] = EFFECT_GRAYSCALE_STR;
	mlx->effect_str[4] = EFFECT_BLACK_WHITE_STR;
	mlx->curr_effect = mlx->effect_str[0];
	mlx->effect_i = 0;

	mlx->negative = 0;
	mlx->negative_str[0] = NEGATIVE_OFF_STR;
	mlx->negative_str[1] = NEGATIVE_ON_STR;

	mlx->noise = 0;
	mlx->noise_str[0] = NOISE_OFF_STR;
	mlx->noise_str[1] = NOISE_ON_STR;

	mlx->colored_light = 0;
	mlx->colored_light_str[0] = COLORED_LIGHT_OFF_STR;
	mlx->colored_light_str[1] = COLORED_LIGHT_ON_STR;

	mlx->soft_shadows = 0;
	mlx->ss_cell = 16;
	mlx->soft_sh_str[0] = SOFT_SHADOWS_OFF_STR;
	mlx->soft_sh_str[1] = SOFT_SHADOWS_ON_STR;

	mlx->bw_factor = 0;
	mlx->ns_factor = 16;

	mlx->aa_dir = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->aa_dir_cpy = (t_vec3 *)malloc(sizeof(t_vec3));

	mlx->aa_idx = 0;
	mlx->aa_val[0] = 0;
	mlx->aa_val[1] = 1;
	mlx->aa_val[2] = 2;
	mlx->aa_val[3] = 4;
	
	mlx->aa_str[0] = AA_0;
	mlx->aa_str[1] = AA_1;
	mlx->aa_str[2] = AA_2;
	mlx->aa_str[3] = AA_3;

	mlx->aa_misc.x = (double)1.0f / (double)W;
	mlx->aa_misc.y = (double)1.0f / (double)H;
	mlx->aa_misc.z = (int)mlx->aa_val[mlx->aa_idx];

	mlx->norm_mapping = 1;
	mlx->norm_str[0] = NORM_OFF;
	mlx->norm_str[1] = NORM_ON;


	ft_init_gpu(mlx);
	ft_load_cl_files(mlx);
}

int		main()
{
    t_mlx *mlx;

    mlx = (t_mlx*)malloc(sizeof(t_mlx));
    mlx->mlx = mlx_init();
    mlx->win = mlx_new_window(mlx->mlx, W, H, "RTv1");
    mlx->img = mlx_new_image(mlx->mlx, W, H);
    mlx->data = (int*)mlx_get_data_addr(mlx->img, &mlx->bpp, &mlx->size_line, &mlx->endian);

    ft_init(mlx);

		mlx_loop_hook(mlx->mlx, ft_gameloop, mlx);
	mlx_hook(mlx->win, 2, 1L << 0, ft_key_press, mlx);
	mlx_hook(mlx->win, 3, 1L << 1, ft_key_realese, mlx);
    mlx_hook(mlx->win, 17, 0, ft_close, (void *)1);
	mlx_loop(mlx->mlx);
    return (0);
}
