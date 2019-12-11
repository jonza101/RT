/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 00:55:32 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/11/10 19:04:27 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_key_realese(int keycode, t_mlx *mlx)
{
	if (keycode == MAC_W || keycode == 119)
		mlx->wsad[0] = 0;
	if (keycode == MAC_S || keycode == 115)
		mlx->wsad[1] = 0;
	if (keycode == MAC_A || keycode == 97)
		mlx->wsad[2] = 0;
	if (keycode == MAC_D || keycode == 100)
		mlx->wsad[3] = 0;

	if (keycode == MAC_SHIFT_L || keycode == 65505)
		mlx->shift = 0;

	if (keycode == MAC_UP || keycode == 65362)
		mlx->arrow[0] = 0;
	if (keycode == MAC_DOWN || keycode == 65364)
		mlx->arrow[1] = 0;
	if (keycode == MAC_LEFT || keycode == 65361)
		mlx->arrow[2] = 0;
	if (keycode == MAC_RIGHT || keycode == 65363)
		mlx->arrow[3] = 0;

	if (keycode == MAC_SPACE || keycode == 32)
		mlx->up_down[0] = 0;
	if (keycode == MAC_CTRL_L || keycode == 65507)
		mlx->up_down[1] = 0;

	return (0);
}

int		ft_key_press(int keycode, t_mlx *mlx)
{
	// printf("%d\n", keycode);
    (keycode == MAC_ESC || keycode == 65307) ? exit(0) : 1;
	if (keycode == MAC_W || keycode == 119)
		mlx->wsad[0] = 1;
	if (keycode == MAC_S || keycode == 115)
		mlx->wsad[1] = 1;
	if (keycode == MAC_A || keycode == 97)
		mlx->wsad[2] = 1;
	if (keycode == MAC_D || keycode == 100)
		mlx->wsad[3] = 1;

	if (keycode == MAC_SHIFT_L || keycode == 65505)
		mlx->shift = 1;

    if (keycode == MAC_UP || keycode == 65362)
		mlx->arrow[0] = 1;
	if (keycode == MAC_DOWN || keycode == 65364)
		mlx->arrow[1] = 1;
	if (keycode == MAC_LEFT || keycode == 65361)
		mlx->arrow[2] = 1;
	if (keycode == MAC_RIGHT || keycode == 65363)
		mlx->arrow[3] = 1;

	if (keycode == MAC_SPACE || keycode == 32)
		mlx->up_down[0] = 1;
	if (keycode == MAC_CTRL_L || keycode == 65507)
		mlx->up_down[1] = 1;

	if (keycode == MAC_G || keycode == 103)
	{
		if (mlx->gpu)
		{
			mlx->render_func = ft_render;
			mlx->render_device = CPU_STR;
		}
		else
		{
			mlx->render_func = ft_execute_kernel;
			mlx->render_device = GPU_STR;
		}
		mlx->gpu = !mlx->gpu;
	}

	if (keycode == MAC_C || keycode == 99)
	{
		mlx->effect_i++;
		if (mlx->effect_i >= EFFECTS)
			mlx->effect_i = 0;
		mlx->curr_effect = mlx->effect_str[mlx->effect_i];
	}
	if ((keycode == MAC_PLUS || keycode == 61) && mlx->cel_band < 5)
		mlx->cel_band++;
	if ((keycode == MAC_MINUS || keycode == 45) && mlx->cel_band > 1)
		mlx->cel_band--;

	if (keycode == MAC_X || keycode == 120)
		mlx->colored_light = !mlx->colored_light;

    return (0);
}