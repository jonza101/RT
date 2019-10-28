/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 00:20:36 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/29 00:53:08 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	ft_move(t_mlx *mlx)
{
	if (mlx->wsad[0])
	{
		mlx->cam->x +=( sinf(mlx->dx) * CAM_SPEED);
		mlx->cam->z += (cosf(mlx->dx) * CAM_SPEED);
		mlx->cam->y += (tanf(mlx->dy) * CAM_SPEED);
	}
	if (mlx->wsad[1])
	{
		mlx->cam->x -= (sinf(mlx->dx) * CAM_SPEED);
		mlx->cam->z -= (cosf(mlx->dx) * CAM_SPEED);
		mlx->cam->y -= (tanf(mlx->dy) * CAM_SPEED);
	}
	if (mlx->wsad[2])
	{
		mlx->cam->x -= (cosf(mlx->dx) * CAM_SPEED);
		mlx->cam->z += (sinf(mlx->dx) * CAM_SPEED);
	}
	if (mlx->wsad[3])
	{
		mlx->cam->x += (cosf(mlx->dx) * CAM_SPEED);
		mlx->cam->z -= (sinf(mlx->dx) * CAM_SPEED);
	}

	if (mlx->arrow[0])
		mlx->dy += VERTICAL_SENS;
	if (mlx->arrow[1])
		mlx->dy -= VERTICAL_SENS;
	if (mlx->arrow[2])
		mlx->dx -= HORIZONTAL_SENS;
	if (mlx->arrow[3])
		mlx->dx += HORIZONTAL_SENS;
	// if (mlx->dx < -3.14159f)
	// 	mlx->dx += (2.0f * 3.14159f);
	// if (mlx->dx > 3.14159f)
	// 	mlx->dx -= (2.0f * 3.14159f);

	if (mlx->up_down[0])
		mlx->cam->y += CAM_VERT_SPEED;
	if (mlx->up_down[1])
		mlx->cam->y -= CAM_VERT_SPEED;
}
