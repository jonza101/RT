/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:42:38 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/24 13:13:07 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_close(int i)
{
	i = 1;
	exit(0);
}

int		ft_key_press(int keycode, t_mlx *mlx)
{
    (keycode == MAC_ESC) ? exit(0) : 1;
	if (keycode == MAC_A)
	{
		mlx->cam->x -= 0.1f;
		printf("x %f\n", mlx->cam->x);
	}
	if (keycode == MAC_D)
	{
		mlx->cam->x += 0.1f;
		printf("x %f\n", mlx->cam->x);
	}
	if (keycode == MAC_W)
	{
		mlx->cam->z += 0.1f;
		printf("z %f\n", mlx->cam->z);
	}
	if (keycode == MAC_S)
	{
		mlx->cam->z -= 0.1f;
		printf("z %f\n", mlx->cam->z);
	}
    return (0);
}

int		ft_gameloop(t_mlx *mlx)
{
    ft_render(mlx);

    mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
    return (0);
}

void	ft_init(t_mlx *mlx)
{
    mlx->cam = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->cam->x = 0.0f;
    mlx->cam->y = 0.0f;
    mlx->cam->z = 0.0f;

	mlx->oc = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->dir = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->point = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->normal = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light_dir = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->neg_dir = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->s_refl = (t_vec3*)malloc(sizeof(t_vec3));


	mlx->obj_count = 5;
	mlx->obj = (t_obj**)malloc(sizeof(t_obj*) * mlx->obj_count);

	int i = -1;
	while (++i < mlx->obj_count)
	{
		mlx->obj[i] = (t_obj*)malloc(sizeof(t_obj));
		mlx->obj[i]->c = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->obj[i]->normal = NULL;
		if (i > 2)
			mlx->obj[i]->normal = (t_vec3*)malloc(sizeof(t_vec3));
	}

	mlx->obj[0]->c->x = 0.0f;
	mlx->obj[0]->c->y = 0.0f;
	mlx->obj[0]->c->z = 5.0f;
	mlx->obj[0]->radius = 0.5f;
	mlx->obj[0]->color = 0x00CCFF;
	mlx->obj[0]->specular = 0.0f;
	mlx->obj[0]->intersect = ft_sph_intersect;
	mlx->obj[0]->normal_calc = ft_sph_normal_calc;

	mlx->obj[1]->c->x = -1.5f;
	mlx->obj[1]->c->y = 1.0f;
	mlx->obj[1]->c->z = 5.5f;
	mlx->obj[1]->radius = 0.75f;
	mlx->obj[1]->color = 0x00CC99;
	mlx->obj[1]->specular = 75.0f;
	mlx->obj[1]->intersect = ft_sph_intersect;
	mlx->obj[1]->normal_calc = ft_sph_normal_calc;

	mlx->obj[2]->c->x = -1.0f;
	mlx->obj[2]->c->y = -1.0f;
	mlx->obj[2]->c->z = 5.75f;
	mlx->obj[2]->radius = 1.25f;
	mlx->obj[2]->color = 0xBD0052;
	mlx->obj[2]->specular = 1000.0f;
	mlx->obj[2]->intersect = ft_sph_intersect;
	mlx->obj[2]->normal_calc = ft_sph_normal_calc;

	mlx->obj[3]->c->x = 0.0f;
	mlx->obj[3]->c->y = -1.25f;
	mlx->obj[3]->c->z = 25.0f;
	mlx->obj[3]->normal->x = 0.0f;
	mlx->obj[3]->normal->y = 0.0f;
	mlx->obj[3]->normal->z = -1.0f;
	mlx->obj[3]->normal = ft_vec_normalize(mlx->obj[3]->normal);
	mlx->obj[3]->color = 0xFFFFFF;
	mlx->obj[3]->specular = 0.0f;
	mlx->obj[3]->intersect = ft_plane_intersect;
	mlx->obj[3]->normal_calc = ft_plane_normal_calc;

	mlx->obj[4]->c->x = 2.0f;
	mlx->obj[4]->c->y = 1.0f;
	mlx->obj[4]->c->z = 5.0f;
	mlx->obj[4]->radius = 0.25f;
	mlx->obj[4]->normal->x = 0.0f;
	mlx->obj[4]->normal->y = 1.0f;
	mlx->obj[4]->normal->z = 0.0f;
	mlx->obj[4]->color = 0x8CFF00;
	mlx->obj[4]->specular = 750.0f;
	mlx->obj[4]->intersect = ft_cone_intersect;
	mlx->obj[4]->normal_calc = ft_cone_normal_calc;


	mlx->light_count = 3;
	mlx->light = (t_light**)malloc(sizeof(t_light*) * mlx->light_count);

	mlx->light[0] = (t_light*)malloc(sizeof(t_light));
	mlx->light[0]->type = 0;
	mlx->light[0]->intensity = 0.0f;
	mlx->light[0]->vec = NULL;

	mlx->light[1] = (t_light*)malloc(sizeof(t_light));
	mlx->light[1]->type = 1;
	mlx->light[1]->intensity = 0.45f;
	mlx->light[1]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[1]->vec->x = 0.0f;
	mlx->light[1]->vec->y = 3.0f;
	mlx->light[1]->vec->z = 0.0f;

	mlx->light[2] = (t_light*)malloc(sizeof(t_light));
	mlx->light[2]->type = 1;
	mlx->light[2]->intensity = 0.25f;
	mlx->light[2]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[2]->vec->x = -3.25f;
	mlx->light[2]->vec->y = 2.5f;
	mlx->light[2]->vec->z = 4.0f;
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
    mlx_hook(mlx->win, 17, 0, ft_close, (void *)1);
	mlx_loop(mlx->mlx);
    return (0);
}
