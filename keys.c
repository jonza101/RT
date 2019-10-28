/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 00:55:32 by zjeyne-l          #+#    #+#             */
/*   Updated: 2019/10/29 00:55:45 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_key_realese(int keycode, t_mlx *mlx)
{
	if (keycode == MAC_W || keycode == 1731)
		mlx->wsad[0] = 0;
	if (keycode == MAC_S || keycode == 1753)
		mlx->wsad[1] = 0;
	if (keycode == MAC_A || keycode == 1734)
		mlx->wsad[2] = 0;
	if (keycode == MAC_D || keycode == 1751)
		mlx->wsad[3] = 0;

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
	if (keycode == MAC_W || keycode == 1731)
		mlx->wsad[0] = 1;
	if (keycode == MAC_S || keycode == 1753)
		mlx->wsad[1] = 1;
	if (keycode == MAC_A || keycode == 1734)
		mlx->wsad[2] = 1;
    if (keycode == MAC_D || keycode == 1751)
		mlx->wsad[3] = 1;

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

    return (0);
}
