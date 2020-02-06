/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 20:15:28 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/02/03 21:25:46 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	ft_strsplit_free(char **split)
{
	int i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

void	ft_init_txt(t_mlx *mlx)
{
	int		fd;
	char	*line;

	char *txt[TXT] = {"textures/venus.xpm", "textures/earth.xpm", "textures/earth_night.xpm", "textures/moon.xpm",
					"textures/mars.xpm", "textures/jupiter.xpm", "textures/saturn.xpm", "textures/p_cer.xpm",
					"textures/m_scuff.xpm", "textures/brick.xpm"};

	int i = -1;
	while (++i < TXT)
	{
		fd = open(txt[i], O_RDONLY);
		int j = -1;
		while (++j < 4)
		{
			get_next_line(fd, &line);
			(j < 3) ? ft_strdel(&line) : 1;
		}
		char **tmp = ft_strsplit(line, ' ');
		mlx->txt[i] = (t_img *)malloc(sizeof(t_img));
		mlx->txt[i]->w = atoi(&tmp[0][1]);
		mlx->txt[i]->h = atoi(tmp[1]);
		mlx->txt[i]->txt_idx = i;
		mlx->txt[i]->img = mlx_xpm_file_to_image(mlx->mlx, txt[i], &mlx->txt[i]->w, &mlx->txt[i]->h);
		mlx->txt[i]->data = (int*)mlx_get_data_addr(mlx->txt[i]->img, &mlx->txt[i]->bpp, &mlx->txt[i]->size_line, &mlx->txt[i]->endian);
		ft_strsplit_free(tmp);
		ft_strdel(&line);
		close(fd);
	}
}
