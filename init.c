/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 20:15:28 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/02/19 21:15:44 by zjeyne-l         ###   ########.fr       */
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

	char *txt[TXT] = {"textures/wbg_tile.xpm", "textures/brick.xpm", "textures/g_tile.xpm",
						"textures/paint.xpm"};

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
		mlx->txt[i] = (t_img*)malloc(sizeof(t_img));
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

void	ft_init_bump(t_mlx *mlx)
{
	int 	fd;
	char 	*line;

	char	*bump[BUMP] = {"textures/bump/wbg_tile_bump.xpm", "textures/bump/brick_bump.xpm",
							"textures/bump/g_tile_bump.xpm", "textures/bump/paint_bump.xpm"};

	int i = -1;
	while (++i < BUMP)
	{
		fd = open(bump[i], O_RDONLY);
		int j = -1;
		while (++j < 4)
		{
			get_next_line(fd, &line);
			(j < 3) ? ft_strdel(&line) : 1;
		}
		char **tmp = ft_strsplit(line, ' ');
		mlx->bump[i] = (t_img*)malloc(sizeof(t_img));
		mlx->bump[i]->w = atoi(&tmp[0][1]);
		mlx->bump[i]->h = atoi(tmp[1]);
		mlx->bump[i]->txt_idx = i;
		mlx->bump[i]->img = mlx_xpm_file_to_image(mlx->mlx, bump[i], &mlx->bump[i]->w, &mlx->bump[i]->h);
		mlx->bump[i]->data = (int *)mlx_get_data_addr(mlx->bump[i]->img, &mlx->bump[i]->bpp, &mlx->bump[i]->size_line, &mlx->bump[i]->endian);
		ft_strsplit_free(tmp);
		ft_strdel(&line);
		close(fd);
	}
}

void	ft_init_rgh(t_mlx *mlx)
{
	int		fd;
	char	*line;

	char	*rgh[RGH] = {"textures/rgh/wbg_tile_rgh.xpm", "textures/rgh/brick_rgh.xpm",
						"textures/rgh/g_tile_rgh.xpm", "textures/rgh/paint_rgh.xpm"};

	int i = -1;
	while (++i < RGH)
	{
		fd = open(rgh[i], O_RDONLY);
		int j = -1;
		while (++j < 4)
		{
			get_next_line(fd, &line);
			(j < 3) ? ft_strdel(&line) : 1;
		}
		char **tmp = ft_strsplit(line, ' ');
		mlx->rgh[i] = (t_img*)malloc(sizeof(t_img));
		mlx->rgh[i]->w = atoi(&tmp[0][1]);
		mlx->rgh[i]->h = atoi(tmp[1]);
		mlx->rgh[i]->txt_idx = i;
		mlx->rgh[i]->img = mlx_xpm_file_to_image(mlx->mlx, rgh[i], &mlx->rgh[i]->w, &mlx->rgh[i]->h);
		mlx->rgh[i]->data = (int *)mlx_get_data_addr(mlx->rgh[i]->img, &mlx->rgh[i]->bpp, &mlx->rgh[i]->size_line, &mlx->rgh[i]->endian);
		ft_strsplit_free(tmp);
		ft_strdel(&line);
		close(fd);
	}
	
}
