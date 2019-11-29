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

void	ft_rotate_x(t_vec3 *vec, float angle, t_vec3 *temp)
{
	float cos_ang = cosf(angle);
	float sin_ang = sinf(angle);

	temp->x = vec->x;
	temp->y = vec->y;
	temp->z = vec->z;

	vec->x = temp->x;
	vec->y = (temp->y * cos_ang) - (temp->z * sin_ang);
	vec->z = (temp->y * sin_ang) + (temp->z * cos_ang);
}

void	ft_rotate_y(t_vec3 *vec, float angle, t_vec3 *temp)
{
	float cos_ang = cosf(angle);
	float sin_ang = sinf(angle);

	temp->x = vec->x;
	temp->y = vec->y;
	temp->z = vec->z;

	vec->x = (temp->x * cos_ang) + (temp->z * sin_ang);
	vec->y = temp->y;
	vec->z = (-temp->x * sin_ang) + (temp->z * cos_ang);
}

t_vec3	*ft_vec_rotate(t_vec3 *vec, float dx, float dy, t_vec3 *temp)
{
	ft_rotate_x(vec, dy, temp);
	ft_rotate_y(vec, dx, temp);
	vec = ft_vec_normalize(vec);
	return (vec);
}

void	ft_move(t_mlx *mlx)
{
	
	float accelerate = (mlx->shift) ? CAM_ACC_SPEED : 1.0f;
	if (mlx->wsad[0])
	{
		mlx->cam->x += (sinf(mlx->dx) * CAM_SPEED * accelerate);
		mlx->cam->z += (cosf(mlx->dx) * CAM_SPEED * accelerate);
		mlx->cam->y -= (tanf(mlx->dy) * CAM_SPEED * accelerate);
	}
	if (mlx->wsad[1])
	{
		mlx->cam->x -= (sinf(mlx->dx) * CAM_SPEED * accelerate);
		mlx->cam->z -= (cosf(mlx->dx) * CAM_SPEED * accelerate);
		mlx->cam->y += (tanf(mlx->dy) * CAM_SPEED * accelerate);
	}
	if (mlx->wsad[2])
	{
		mlx->cam->x -= (cosf(mlx->dx) * CAM_SPEED * accelerate);
		mlx->cam->z += (sinf(mlx->dx) * CAM_SPEED * accelerate);
	}
	if (mlx->wsad[3])
	{
		mlx->cam->x += (cosf(mlx->dx) * CAM_SPEED * accelerate);
		mlx->cam->z -= (sinf(mlx->dx) * CAM_SPEED * accelerate);
	}

	if (mlx->arrow[0])
		mlx->dy -= VERTICAL_SENS;
	if (mlx->arrow[1])
		mlx->dy += VERTICAL_SENS;
	if (mlx->arrow[2])
		mlx->dx -= HORIZONTAL_SENS;
	if (mlx->arrow[3])
		mlx->dx += HORIZONTAL_SENS;
	if (mlx->dx < -3.14159f)
		mlx->dx += (2.0f * 3.14159f);
	if (mlx->dx > 3.14159f)
		mlx->dx -= (2.0f * 3.14159f);

	mlx->dy = ft_clamp(mlx->dy, MIN_VERT_CAM_VAL, MAX_VERT_CAM_VAL);

	if (mlx->up_down[0])
		mlx->cam->y += CAM_VERT_SPEED;
	if (mlx->up_down[1])
		mlx->cam->y -= CAM_VERT_SPEED;
}
