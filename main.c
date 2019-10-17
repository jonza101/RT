/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:42:38 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/17 18:10:20 by zjeyne-l         ###   ########.fr       */
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

	mlx->l_point = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->l_normal = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->l_vec = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->s_dir = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->s_refl = (t_vec3*)malloc(sizeof(t_vec3));

	mlx->sph = (t_sphere**)malloc(sizeof(t_sphere*) * 3);

    mlx->sph[0] = (t_sphere*)malloc(sizeof(t_sphere));
    mlx->sph[0]->center = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->sph[0]->center->x = 0.0f;
    mlx->sph[0]->center->y = 0.0f;
    mlx->sph[0]->center->z = 5.0f;
    mlx->sph[0]->radius = 0.5f;
    mlx->sph[0]->color = 0xFFFF00;
	mlx->sph[0]->specular = 0.0f;

	mlx->sph[1] = (t_sphere*)malloc(sizeof(t_sphere));
    mlx->sph[1]->center = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->sph[1]->center->x = -1.5f;
    mlx->sph[1]->center->y = -1.0f;
    mlx->sph[1]->center->z = 5.5f;
    mlx->sph[1]->radius = 0.75f;
    mlx->sph[1]->color = 0xFF0000;
	mlx->sph[1]->specular = 250.0f;

	mlx->sph[2] = (t_sphere*)malloc(sizeof(t_sphere));
    mlx->sph[2]->center = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->sph[2]->center->x = -1.0f;
    mlx->sph[2]->center->y = 1.0f;
    mlx->sph[2]->center->z = 5.75f;
    mlx->sph[2]->radius = 1.25f;
    mlx->sph[2]->color = 0xFF00FF;
	mlx->sph[2]->specular = 2500.0f;

	mlx->light = (t_light**)malloc(sizeof(t_light*) * 3);

	mlx->light[0] = (t_light*)malloc(sizeof(t_light));
	mlx->light[0]->type = 0;
	mlx->light[0]->intensity = 0.0f;
	mlx->light[0]->vec = NULL;

	mlx->light[1] = (t_light*)malloc(sizeof(t_light));
	mlx->light[1]->type = 2;
	mlx->light[1]->intensity = 0.45f;
	mlx->light[1]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[1]->vec->x = 4.0f;
	mlx->light[1]->vec->y = 4.0f;
	mlx->light[1]->vec->z = 5.0f;

	mlx->light[2] = (t_light*)malloc(sizeof(t_light));
	mlx->light[2]->type = 1;
	mlx->light[2]->intensity = 0.55f;
	mlx->light[2]->vec = (t_vec3*)malloc(sizeof(t_vec3));
	mlx->light[2]->vec->x = 0.0f;
	mlx->light[2]->vec->y = -2.0f;
	mlx->light[2]->vec->z = 1.0f;
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
