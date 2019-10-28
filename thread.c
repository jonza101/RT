/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 23:36:54 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/29 00:49:22 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	ft_thread_init(t_mlx *mlx)
{
	int i = -1;
	while (++i < THREAD)
	{
		mlx->mlx_thread[i] = (t_mlx*)malloc(sizeof(t_mlx));
		mlx->mlx_thread[i]->dir = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->oc = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->point = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->normal = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->light_dir = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->neg_dir = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->s_refl = (t_vec3*)malloc(sizeof(t_vec3));
		mlx->mlx_thread[i]->obj_count = mlx->obj_count;
		mlx->mlx_thread[i]->obj = mlx->obj;
		mlx->mlx_thread[i]->light_count = mlx->light_count;
		mlx->mlx_thread[i]->light = mlx->light;
		mlx->mlx_thread[i]->thread_i = i;
		mlx->mlx_thread[i]->data = mlx->data;
	}
}

void	ft_thread(t_mlx *mlx)
{
	pthread_t	thread[THREAD];

	int i = -1;
	while (++i < THREAD)
	{
		mlx->mlx_thread[i]->cam = mlx->cam;
		mlx->mlx_thread[i]->dx = mlx->dx;
		mlx->mlx_thread[i]->dy = mlx->dy;
		pthread_create(&thread[i], NULL, (void*)ft_render, (void*)mlx->mlx_thread[i]);
	}
	i = -1;
	while (++i < THREAD)
		pthread_join(thread[i], NULL);
}
