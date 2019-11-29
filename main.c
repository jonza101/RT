/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:42:38 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/11/10 19:15:23 by zjeyne-l         ###   ########.fr       */
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

	double delta_time =  ((double)(start_time - mlx->last_time) / CLOCKS_PER_SEC);
	int fps = (double)1.0f / (double)delta_time;
	char *fps_str = ft_itoa(fps);

	if (fps <= MAX_FPS)
	{
		mlx->last_time = start_time;

		ft_move(mlx);
		// ft_render(mlx);
		mlx->render_func(mlx);

		mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);

		
		mlx_string_put(mlx->mlx, mlx->win, 10, 20, 0xFFFFFF, fps_str);
		mlx_string_put(mlx->mlx, mlx->win, 10, 40, 0xFFFFFF, mlx->render_device);
	}
	free(fps_str);

    return (0);
}

void	ft_init(t_mlx *mlx)
{
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


	mlx->obj_count = 8;
	mlx->obj = (t_obj**)malloc(sizeof(t_obj*) * mlx->obj_count);

	int i = -1;
	while (++i < mlx->obj_count)
	{
		mlx->obj[i] = (t_obj*)malloc(sizeof(t_obj));
		mlx->obj[i]->c = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->normal = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->oc_temp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->p0 = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->p1 = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->p2 = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->vec_temp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->vec_tmp = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->t_p = (t_vec3*)malloc(sizeof(t_vec3));
	}
	mlx->obj_count = 7;

	mlx->obj[0]->type = SPHERE;
	mlx->obj[0]->c->x = 0.0f;
	mlx->obj[0]->c->y = 0.0f;
	mlx->obj[0]->c->z = 5.0f;
	mlx->obj[0]->radius = 0.5f;
	mlx->obj[0]->color = 0x00CCFF;
	mlx->obj[0]->specular = 0.0f;
	mlx->obj[0]->intersect = ft_sph_intersect;
	mlx->obj[0]->normal_calc = ft_sph_normal_calc;
	mlx->obj[0]->mirrored = 0.0f;

	mlx->obj[1]->type = SPHERE;
	mlx->obj[1]->c->x = -1.5f;
	mlx->obj[1]->c->y = 1.0f;
	mlx->obj[1]->c->z = 5.5f;
	mlx->obj[1]->radius = 0.75f;
	mlx->obj[1]->color = 0x00CC99;
	mlx->obj[1]->specular = 75.0f;
	mlx->obj[1]->intersect = ft_sph_intersect;
	mlx->obj[1]->normal_calc = ft_sph_normal_calc;
    mlx->obj[1]->mirrored = 0.4f;

	mlx->obj[2]->type = SPHERE;
	mlx->obj[2]->c->x = -1.0f;
	mlx->obj[2]->c->y = -1.0f;
	mlx->obj[2]->c->z = 5.75f;
	mlx->obj[2]->radius = 1.25f;
	mlx->obj[2]->color = 0xBD0052;
	mlx->obj[2]->specular = 500.0f;
	mlx->obj[2]->intersect = ft_sph_intersect;
	mlx->obj[2]->normal_calc = ft_sph_normal_calc;
    mlx->obj[2]->mirrored = 0.5f;

	mlx->obj[3]->type = PLANE;
	mlx->obj[3]->c->x = 0.0f;
	mlx->obj[3]->c->y = -1.25f;
	mlx->obj[3]->c->z = 25.0f;
	mlx->obj[3]->normal->x = 0.0f;
	mlx->obj[3]->normal->y = 1.0f;
	mlx->obj[3]->normal->z = 0.0f;
	mlx->obj[3]->color = 0xFFFFFF;
	mlx->obj[3]->specular = 0.0f;
	mlx->obj[3]->intersect = ft_plane_intersect;
	mlx->obj[3]->normal_calc = ft_plane_normal_calc;
    mlx->obj[3]->mirrored = 0.75f;

	mlx->obj[4]->type = CONE;
	mlx->obj[4]->c->x = 3.5f;
	mlx->obj[4]->c->y = 1.0f;
	mlx->obj[4]->c->z = 7.5f;
	mlx->obj[4]->radius = 0.25f;
	mlx->obj[4]->normal->x = 0.5f;
	mlx->obj[4]->normal->y = 1.0f;
	mlx->obj[4]->normal->z = 0.75f;
	mlx->obj[4]->normal = ft_vec_normalize(mlx->obj[4]->normal);
	mlx->obj[4]->color = 0x8fff00;
	mlx->obj[4]->specular = 750.0f;
	mlx->obj[4]->intersect = ft_cone_intersect;
	mlx->obj[4]->normal_calc = ft_cone_normal_calc;
    mlx->obj[4]->mirrored = 0.1f;

	mlx->obj[5]->type = CYLINDER;
    mlx->obj[5]->c->x = -7.0f;
    mlx->obj[5]->c->y = 0.0f;
    mlx->obj[5]->c->z = 15.0f;
    mlx->obj[5]->radius = 0.75f;
    mlx->obj[5]->normal->x = -0.2f;
    mlx->obj[5]->normal->y = 1.0f;
    mlx->obj[5]->normal->z = 0.0f;
	mlx->obj[5]->normal = ft_vec_normalize(mlx->obj[5]->normal);
    mlx->obj[5]->color = 0xFF00FF;
    mlx->obj[5]->specular = 750.0f;
    mlx->obj[5]->intersect = ft_cylinder_intersect;
    mlx->obj[5]->normal_calc = ft_cylinder_normal_calc;
    mlx->obj[5]->mirrored = 0.0f;

	mlx->obj[6]->type = PLANE;
	mlx->obj[6]->c->x = 0.0f;
	mlx->obj[6]->c->y = 0.0f;
	mlx->obj[6]->c->z = 50.0f;
	mlx->obj[6]->normal->x = 0.0f;
	mlx->obj[6]->normal->y = 0.0f;
	mlx->obj[6]->normal->z = 1.0f;
	mlx->obj[6]->color = 0x8D41D9;
	mlx->obj[6]->specular = 0.0f;
	mlx->obj[6]->intersect = ft_plane_intersect;
	mlx->obj[6]->normal_calc = ft_plane_normal_calc;
    mlx->obj[6]->mirrored = 0.0f;

	mlx->obj[7]->type = TRIANGLE;
	mlx->obj[7]->p0->x = -4.0f;
	mlx->obj[7]->p0->y = 1.0f;
	mlx->obj[7]->p0->z = 4.0f;
	mlx->obj[7]->p1->x = -3.0f;
	mlx->obj[7]->p1->y = 1.75f;
	mlx->obj[7]->p1->z = 6.0f;
	mlx->obj[7]->p2->x = -3.5f;
	mlx->obj[7]->p2->y = 0.0f;
	mlx->obj[7]->p2->z = 4.5f;
	mlx->obj[7]->normal = ft_triangle_normal_calc(mlx->obj[7]->normal, mlx->obj[7]->normal, mlx->obj[7]->normal, mlx->obj[7]);
	mlx->obj[7]->color = 0xA8EEFF;
	mlx->obj[7]->specular = 0.0f;
	mlx->obj[7]->intersect = ft_triangle_intersect;
	mlx->obj[7]->normal_calc = ft_triangle_normal_calc;
    mlx->obj[7]->mirrored = 0.2f;


	mlx->light_count = 5;
	mlx->light = (t_light**)malloc(sizeof(t_light*) * mlx->light_count);
	i = -1;
	while (++i < mlx->light_count)
	{
		mlx->light[i] = (t_light*)malloc(sizeof(t_light));
		mlx->light[i]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	}

	mlx->light[0] = (t_light*)malloc(sizeof(t_light));
	mlx->light[0]->type = AMBIENT_L;
	mlx->light[0]->intensity = 0.0f;
	mlx->light[0]->vec = NULL;

	mlx->light[1] = (t_light*)malloc(sizeof(t_light));
	mlx->light[1]->type = POINT_L;
	mlx->light[1]->intensity = 0.45f;
	mlx->light[1]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[1]->vec->x = 0.0f;
	mlx->light[1]->vec->y = 3.0f;
	mlx->light[1]->vec->z = 0.0f;

	mlx->light[2] = (t_light*)malloc(sizeof(t_light));
	mlx->light[2]->type = POINT_L;
	mlx->light[2]->intensity = 0.25f;
	mlx->light[2]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[2]->vec->x = -3.25f;
	mlx->light[2]->vec->y = 2.5f;
	mlx->light[2]->vec->z = 4.0f;

	mlx->light[3] = (t_light*)malloc(sizeof(t_light));
	mlx->light[3]->type = POINT_L;
	mlx->light[3]->intensity = 0.85f;
	mlx->light[3]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[3]->vec->x = 0.0f;
	mlx->light[3]->vec->y = 7.5f;
	mlx->light[3]->vec->z = 10.0f;


	i = -1;
	while (++i < 4)
	{
		mlx->wsad[i] = 0;
		mlx->arrow[i] = 0;
	}
	mlx->shift = 0;
	mlx->last_time = 0.0f;
	mlx->gpu = 1;
	mlx->render_func = ft_execute_kernel;
	mlx->render_device = "GPU";

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
