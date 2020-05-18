/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 23:36:16 by zjeyne-l          #+#    #+#             */
/*   Updated: 2020/05/19 00:09:27 by zjeyne-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	ft_device_info(t_mlx *mlx)
{
	char d_name[1024];
	char d_vendor[1024];
	cl_bool d_available;
	cl_uint d_n_cores;
	cl_uint d_clock_freq;
	cl_long d_amount_mem;
	cl_ulong d_max_alloc_mem;
	cl_ulong d_local_mem;
	size_t d_max_group_size;
	size_t d_max_work_item_size;
	cl_uint d_address_bits;

	clGetDeviceInfo(mlx->device_id, CL_DEVICE_NAME, sizeof(d_name), d_name, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_VENDOR, sizeof(d_vendor), d_vendor, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_AVAILABLE, sizeof(d_available), &d_available, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(d_n_cores), &d_n_cores, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(d_clock_freq), &d_clock_freq, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(d_amount_mem), &d_amount_mem, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(d_max_alloc_mem), &d_max_alloc_mem, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(d_local_mem), &d_local_mem, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(d_max_group_size), &d_max_group_size, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(d_max_work_item_size), &d_max_work_item_size, NULL);
	clGetDeviceInfo(mlx->device_id, CL_DEVICE_ADDRESS_BITS, sizeof(d_address_bits), &d_address_bits, NULL);

	printf("\n\n---------------DEVICE_INFO---------------\n");
	printf("\nd_name: %s\n", d_name);
	printf("d_vendor: %s\n", d_vendor);
	printf("d_available: %s\n", d_available ? "Yes" : "No");
	printf("d_num_cores: %u\n", d_n_cores);
	printf("d_clock_freq: %u\n", d_clock_freq);
	printf("d_amount_mem: %f mb\n", (float)d_amount_mem / 1024.0f / 1024.0f);
	printf("d_max_alloc_mem: %f mb\n", (float)d_max_alloc_mem / 1024.0f / 1024.0f);
	printf("d_local_mem: %u kb\n", (unsigned int)((float)d_local_mem / 1024.0f));
	printf("d_max_group_size: %zu\n", d_max_group_size);
	printf("d_max_work_item_size: %zu\n", d_max_work_item_size);
	printf("d_address_bits: %u\n", d_address_bits);
	printf("\n-----------------------------------------\n\n");
}

void	ft_init_gpu_txt(t_mlx *mlx)
{
	mlx->txt_pix = 0;

	int i = -1;
	while (++i < TXT)
	{
		int y = -1;
		while (++y < mlx->txt[i]->h)
		{
			int x = -1;
			while (++x < mlx->txt[i]->w)
			{
				mlx->txt_pix++;
			}
		}
	}

	mlx->obj_txt = (cl_ulong4*)malloc(sizeof(cl_ulong4) * (mlx->txt_pix));
	printf("txt_pix %lu\n", mlx->txt_pix);
	mlx->obj_txt[TXT].w = mlx->txt_pix;
	mlx->obj_txt[0].w = 0;
	mlx->txt_pix = 0;
	int last_wh = 0;
	i = -1;
	while (++i < TXT)
	{
		mlx->obj_txt[i].x = mlx->txt[i]->w;
		mlx->obj_txt[i].y = mlx->txt[i]->h;
		if (i > 0)
			mlx->obj_txt[i].w = (last_wh + (mlx->txt[i - 1]->w * mlx->txt[i - 1]->h));
		last_wh = mlx->obj_txt[i].w;

		int y = -1;
		while (++y < mlx->txt[i]->h)
		{
			int x = -1;
			while (++x < mlx->txt[i]->w)
			{
				mlx->obj_txt[mlx->txt_pix].z = mlx->txt[i]->data[y * mlx->txt[i]->w + x];
				mlx->txt_pix++;
			}
		}
	}


	mlx->norm_pix = 0;

	i = -1;
	while (++i < NORM)
	{
		int y = -1;
		while (++y < mlx->norm[i]->h)
		{
			int x = -1;
			while (++x < mlx->norm[i]->w)
			{
				mlx->norm_pix++;
			}
		}
	}

	mlx->obj_norm_map = (cl_ulong4*)malloc(sizeof(cl_ulong4) * (mlx->norm_pix));
	printf("norm_pix %lu\n", mlx->norm_pix);
	mlx->obj_norm_map[NORM].w = mlx->norm_pix;
	mlx->obj_norm_map[0].w = 0;
	mlx->norm_pix = 0;
	last_wh = 0;
	i = -1;
	while (++i < NORM)
	{
		mlx->obj_norm_map[i].x = mlx->norm[i]->w;
		mlx->obj_norm_map[i].y = mlx->norm[i]->h;
		if (i > 0)
			mlx->obj_norm_map[i].w = (last_wh + (mlx->norm[i - 1]->w * mlx->norm[i - 1]->h));
		last_wh = mlx->obj_norm_map[i].w;

		int y = -1;
		while (++y < mlx->norm[i]->h)
		{
			int x = -1;
			while (++x < mlx->norm[i]->w)
			{
				mlx->obj_norm_map[mlx->norm_pix].z = mlx->norm[i]->data[y * mlx->norm[i]->w + x];
				mlx->norm_pix++;
			}
		}
	}


	mlx->rgh_pix = 0;

	i = -1;
	while (++i < RGH)
	{
		int y = -1;
		while (++y < mlx->rgh[i]->h)
		{
			int x = -1;
			while (++x < mlx->rgh[i]->w)
			{
				mlx->rgh_pix++;
			}
		}
	}

	mlx->obj_rgh_map = (cl_ulong4*)malloc(sizeof(cl_ulong4) * (mlx->rgh_pix));
	printf("rgh_pix %lu\n", mlx->rgh_pix);
	mlx->obj_rgh_map[RGH].w = mlx->rgh_pix;
	mlx->obj_rgh_map[0].w = 0;
	mlx->rgh_pix = 0;
	last_wh = 0;
	i = -1;
	while (++i < RGH)
	{
		mlx->obj_rgh_map[i].x = mlx->rgh[i]->w;
		mlx->obj_rgh_map[i].y = mlx->rgh[i]->h;
		if (i > 0)
			mlx->obj_rgh_map[i].w = (last_wh + (mlx->rgh[i - 1]->w * mlx->rgh[i - 1]->h));
		last_wh = mlx->obj_rgh_map[i].w;

		int y = -1;
		while (++y < mlx->rgh[i]->h)
		{
			int x = -1;
			while (++x < mlx->rgh[i]->w)
			{
				mlx->obj_rgh_map[mlx->rgh_pix].z = mlx->rgh[i]->data[y * mlx->rgh[i]->w + x];
				mlx->rgh_pix++;
			}
		}
	}
}

void	ft_init_gpu_light(t_mlx *mlx)
{
	mlx->light_vec = (cl_float3*)malloc(sizeof(cl_float3) * mlx->light_count);
	mlx->light_type = (cl_int*)malloc(sizeof(cl_int) * mlx->light_count);
	mlx->light_intensity = (cl_float*)malloc(sizeof(cl_float) * mlx->light_count);

	int i = -1;
	while (++i < mlx->light_count)
	{
		if (mlx->light[i]->type != AMBIENT_L)
		{
			mlx->light_vec[i].x = mlx->light[i]->vec->x;
			mlx->light_vec[i].y = mlx->light[i]->vec->y;
			mlx->light_vec[i].z = mlx->light[i]->vec->z;
		}
		mlx->light_type[i] = mlx->light[i]->type;
		mlx->light_intensity[i] = mlx->light[i]->intensity;
	}
}

void	ft_init_gpu_obj(t_mlx *mlx)
{
	mlx->obj_pos = (cl_float3*)malloc(sizeof(cl_float3) * mlx->obj_count);
	mlx->obj_normal = (cl_float3*)malloc(sizeof(cl_float3) * mlx->obj_count);
	mlx->obj_radius = (cl_float*)malloc(sizeof(cl_float) * mlx->obj_count);
	mlx->obj_color = (cl_int*)malloc(sizeof(cl_int) * mlx->obj_count);
	mlx->obj_specular = (cl_float*)malloc(sizeof(cl_float) * mlx->obj_count);
	mlx->obj_mirrored = (cl_float*)malloc(sizeof(cl_float) * mlx->obj_count);
	mlx->obj_transparency = (cl_float*)malloc(sizeof(cl_float) * mlx->obj_count);
	mlx->obj_refractive_index = (cl_float*)malloc(sizeof(cl_float) * mlx->obj_count);
	mlx->obj_type = (cl_int*)malloc(sizeof(cl_int) * mlx->obj_count);
	mlx->obj_txt_norm_rgh_idx = (cl_int3*)malloc(sizeof(cl_int3) * mlx->obj_count);

	int i = -1;
	while (++i < mlx->obj_count)
	{
		mlx->obj_pos[i].x = mlx->obj[i]->c->x;
		mlx->obj_pos[i].y = mlx->obj[i]->c->y;
		mlx->obj_pos[i].z = mlx->obj[i]->c->z;
		mlx->obj_normal[i].x = mlx->obj[i]->normal->x;
		mlx->obj_normal[i].y = mlx->obj[i]->normal->y;
		mlx->obj_normal[i].z = mlx->obj[i]->normal->z;
		mlx->obj_radius[i] = mlx->obj[i]->radius;
		mlx->obj_color[i] = mlx->obj[i]->color;
		mlx->obj_specular[i] = mlx->obj[i]->specular;
		mlx->obj_mirrored[i] = mlx->obj[i]->mirrored;
		mlx->obj_transparency[i] = mlx->obj[i]->transparency;
		mlx->obj_refractive_index[i] = mlx->obj[i]->refractive_index;
		mlx->obj_type[i] = mlx->obj[i]->type;
		mlx->obj_txt_norm_rgh_idx[i].x = (mlx->obj[i]->txt) ? mlx->obj[i]->txt->txt_idx : -1;
		mlx->obj_txt_norm_rgh_idx[i].y = (mlx->obj[i]->norm) ? mlx->obj[i]->norm->txt_idx : -1;
		mlx->obj_txt_norm_rgh_idx[i].z = (mlx->obj[i]->rgh) ? mlx->obj[i]->rgh->txt_idx : -1;
	}
}

void	ft_init_gpu(t_mlx *mlx)
{
	cl_platform_id	platform;

	mlx->ret = clGetPlatformIDs(1, &platform, NULL);
	if (mlx->ret != CL_SUCCESS)
		exit(0);
	mlx->ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &mlx->device_id, NULL);
	if (mlx->ret != CL_SUCCESS)
		exit(0);
	ft_device_info(mlx);
	mlx->contex = clCreateContext(NULL, 1, &mlx->device_id, NULL, NULL, &mlx->ret);
	if (mlx->ret != CL_SUCCESS)
		exit(0);
	mlx->command_queue = clCreateCommandQueue(mlx->contex, mlx->device_id, 0, &mlx->ret);
	if (mlx->ret != CL_SUCCESS)
		exit(0);

	ft_init_gpu_obj(mlx);
	ft_init_gpu_light(mlx);
	ft_init_gpu_txt(mlx);
}

////////////////////////////////////////BUFFER////////////////////////////////////////////
void	ft_obj_buffer(t_mlx *mlx)
{
	mlx->gpu_obj_pos = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float3) * (mlx->obj_count), mlx->obj_pos, &mlx->ret);
	if (!mlx->gpu_obj_pos || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_normal = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float3) * (mlx->obj_count), mlx->obj_normal, &mlx->ret);
	if (!mlx->gpu_obj_normal || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}

	mlx->gpu_obj_radius = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (mlx->obj_count), mlx->obj_radius, &mlx->ret);
	if (!mlx->gpu_obj_radius || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_color = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * (mlx->obj_count), mlx->obj_color, &mlx->ret);
	if (!mlx->gpu_obj_color || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_specular = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (mlx->obj_count), mlx->obj_specular, &mlx->ret);
	if (!mlx->gpu_obj_specular || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_mirrored = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (mlx->obj_count), mlx->obj_mirrored, &mlx->ret);
	if (!mlx->gpu_obj_mirrored || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_transparency = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (mlx->obj_count), mlx->obj_transparency, &mlx->ret);
	if (!mlx->gpu_obj_transparency || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_refractive_index = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (mlx->obj_count), mlx->obj_refractive_index, &mlx->ret);
	if (!mlx->gpu_obj_refractive_index || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_type = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * (mlx->obj_count), mlx->obj_type, &mlx->ret);
	if (!mlx->gpu_obj_type || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}

	mlx->gpu_obj_txt = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong4) * (mlx->txt_pix), mlx->obj_txt, &mlx->ret);
	if (!mlx->gpu_obj_txt || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_txt_norm_rgh_idx = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int3) * (mlx->obj_count), mlx->obj_txt_norm_rgh_idx, &mlx->ret);
	if (!mlx->gpu_txt_norm_rgh_idx || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_norm = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong4) * (mlx->norm_pix), mlx->obj_norm_map, &mlx->ret);
	if (!mlx->gpu_obj_norm || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_obj_rgh = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong4) * (mlx->rgh_pix), mlx->obj_rgh_map, &mlx->ret);
	if (!mlx->gpu_obj_rgh || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}
}

void	ft_light_buffer(t_mlx *mlx)
{
	mlx->gpu_light_vec = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float3) * (mlx->light_count), mlx->light_vec, &mlx->ret);
	if (!mlx->gpu_light_vec || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error4 %d\n", mlx->ret);
		exit(0);
	}

	mlx->gpu_light_type = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * (mlx->light_count), mlx->light_type, &mlx->ret);
	if (!mlx->gpu_light_type || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error4 %d\n", mlx->ret);
		exit(0);
	}
	mlx->gpu_light_intensity = clCreateBuffer(mlx->contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (mlx->light_count), mlx->light_intensity, &mlx->ret);
	if (!mlx->gpu_light_intensity || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error5 %d\n", mlx->ret);
		exit(0);
	}
}

void	ft_create_buffer(t_mlx *mlx)
{
	mlx->buffer = clCreateBuffer(mlx->contex, CL_MEM_WRITE_ONLY, sizeof(cl_uint) * (W * H * 1), NULL, &mlx->ret);
	if (!mlx->buffer || mlx->ret != CL_SUCCESS)
	{
		printf("buffer_create error %d\n", mlx->ret);
		exit(0);
	}

	ft_obj_buffer(mlx);
	ft_light_buffer(mlx);
}
////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////ARGS//////////////////////////////////////////////
void	ft_obj_args(t_mlx *mlx)
{
	mlx->ret = clSetKernelArg(mlx->kernel, 4, sizeof(cl_mem), &mlx->gpu_obj_pos);
	mlx->ret |= clSetKernelArg(mlx->kernel, 5, sizeof(cl_mem), &mlx->gpu_obj_normal);
	mlx->ret |= clSetKernelArg(mlx->kernel, 6, sizeof(cl_mem), &mlx->gpu_obj_radius);
	mlx->ret |= clSetKernelArg(mlx->kernel, 7, sizeof(cl_mem), &mlx->gpu_obj_color);
	mlx->ret |= clSetKernelArg(mlx->kernel, 8, sizeof(cl_mem), &mlx->gpu_obj_specular);
	mlx->ret |= clSetKernelArg(mlx->kernel, 9, sizeof(cl_mem), &mlx->gpu_obj_mirrored);
	mlx->ret |= clSetKernelArg(mlx->kernel, 10, sizeof(cl_mem), &mlx->gpu_obj_transparency);
	mlx->ret |= clSetKernelArg(mlx->kernel, 11, sizeof(cl_mem), &mlx->gpu_obj_refractive_index);
	mlx->ret |= clSetKernelArg(mlx->kernel, 12, sizeof(cl_int), &mlx->obj_count);
	mlx->ret |= clSetKernelArg(mlx->kernel, 13, sizeof(cl_mem), &mlx->gpu_obj_type);
	mlx->ret |= clSetKernelArg(mlx->kernel, 29, sizeof(cl_mem), &mlx->gpu_obj_txt);
	mlx->ret |= clSetKernelArg(mlx->kernel, 30, sizeof(cl_mem), &mlx->gpu_txt_norm_rgh_idx);
	mlx->ret |= clSetKernelArg(mlx->kernel, 32, sizeof(cl_mem), &mlx->gpu_obj_norm);
	mlx->ret |= clSetKernelArg(mlx->kernel, 34, sizeof(cl_mem), &mlx->gpu_obj_rgh);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("kernel_arg error8 %d\n", mlx->ret);
		exit(0);
	}
}

void	ft_light_args(t_mlx *mlx)
{
	mlx->ret = clSetKernelArg(mlx->kernel, 14, sizeof(cl_mem), &mlx->gpu_light_vec);
	mlx->ret |= clSetKernelArg(mlx->kernel, 15, sizeof(cl_mem), &mlx->gpu_light_type);
	mlx->ret |= clSetKernelArg(mlx->kernel, 16, sizeof(cl_mem), &mlx->gpu_light_intensity);
	mlx->ret |= clSetKernelArg(mlx->kernel, 17, sizeof(cl_int), &mlx->light_count);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("kernel_arg error12 %d\n", mlx->ret);
		exit(0);
	}
}

void	ft_set_kernel_args(t_mlx *mlx)
{
	mlx->ret = clSetKernelArg(mlx->kernel, 0, sizeof(cl_mem), &mlx->buffer);
	mlx->ret |= clSetKernelArg(mlx->kernel, 1, sizeof(cl_float), &mlx->cam->x);
	mlx->ret |= clSetKernelArg(mlx->kernel, 2, sizeof(cl_float), &mlx->cam->y);
	mlx->ret |= clSetKernelArg(mlx->kernel, 3, sizeof(cl_float), &mlx->cam->z);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("kernel_arg error %d\n", mlx->ret);
		exit(0);
	}

	ft_obj_args(mlx);
	ft_light_args(mlx);
}
////////////////////////////////////////////////////////////////////////////////////////////////

void	ft_load_cl_files(t_mlx *mlx)
{
	int fd = open("render.cl", O_RDONLY);
	char *line;
	char *kernel_code = ft_strnew(1);
	size_t code_size;
	int line_i = 0;
	while (get_next_line(fd, &line))
	{
		char *temp = kernel_code;
		kernel_code = ft_strjoin(kernel_code, line);
		free(temp);
		temp = kernel_code;
		kernel_code = ft_strjoin(kernel_code, "\n");
		free(temp);
		free(line);
		line_i++;
	}
	code_size = ft_strlen(kernel_code);

	mlx->program = clCreateProgramWithSource(mlx->contex, 1, (const char**)&kernel_code, (const size_t*)&code_size, &mlx->ret);
	if (mlx->ret != CL_SUCCESS)
		{
		printf("program_create error %d\n", mlx->ret);
		exit(0);
	}
	free(kernel_code);

	mlx->ret = clBuildProgram(mlx->program, 1, &mlx->device_id, NULL, NULL, NULL);
	if (mlx->ret != CL_SUCCESS)
	{
		char str[80960];
		clGetProgramBuildInfo(mlx->program, mlx->device_id, CL_PROGRAM_BUILD_LOG, sizeof(str), str, NULL);
		printf("%s\n", str);
		printf("build_program error %d\n", mlx->ret);
		exit(0);
	}
	mlx->kernel = clCreateKernel(mlx->program, "render", &mlx->ret);
	if (!mlx->kernel || mlx->ret != CL_SUCCESS)
	{
		printf("kernel_create error %d\n", mlx->ret);
		exit(0);
	}

	ft_create_buffer(mlx);

	mlx->global_work_size = W * H;
	mlx->ret = clGetKernelWorkGroupInfo(mlx->kernel, mlx->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(mlx->local_work_size), &mlx->local_work_size, 0);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("kernel_work_group error %d\n", mlx->ret);
		exit(0);
	}
	while (mlx->global_work_size % mlx->local_work_size != 0)
		mlx->local_work_size--;
	printf("local_size %ld\n", mlx->local_work_size);

	ft_set_kernel_args(mlx);
}

void	ft_execute_kernel(t_mlx *mlx)
{
	mlx->ret = clSetKernelArg(mlx->kernel, 1, sizeof(cl_float), &mlx->cam->x);
	mlx->ret |= clSetKernelArg(mlx->kernel, 2, sizeof(cl_float), &mlx->cam->y);
	mlx->ret |= clSetKernelArg(mlx->kernel, 3, sizeof(cl_float), &mlx->cam->z);
	mlx->ret |= clSetKernelArg(mlx->kernel, 18, sizeof(cl_float), &mlx->dx);
	mlx->ret |= clSetKernelArg(mlx->kernel, 19, sizeof(cl_float), &mlx->dy);
	mlx->ret |= clSetKernelArg(mlx->kernel, 20, sizeof(cl_int), &mlx->effect_i);
	mlx->ret |= clSetKernelArg(mlx->kernel, 21, sizeof(cl_int), &mlx->cel_band);
	mlx->ret |= clSetKernelArg(mlx->kernel, 22, sizeof(cl_int), &mlx->negative);
	mlx->ret |= clSetKernelArg(mlx->kernel, 23, sizeof(cl_int), &mlx->soft_shadows);
	mlx->ret |= clSetKernelArg(mlx->kernel, 24, sizeof(cl_int), &mlx->ss_cell);
	float seed = (float)rand() / (float)RAND_MAX;
	mlx->ret |= clSetKernelArg(mlx->kernel, 25, sizeof(cl_int), &seed);
	mlx->ret |= clSetKernelArg(mlx->kernel, 26, sizeof(cl_int), &mlx->bw_factor);
	mlx->ret |= clSetKernelArg(mlx->kernel, 27, sizeof(cl_int), &mlx->noise);
	mlx->ret |= clSetKernelArg(mlx->kernel, 28, sizeof(cl_int), &mlx->ns_factor);
	mlx->ret |= clSetKernelArg(mlx->kernel, 31, sizeof(cl_double3), &mlx->aa_misc);
	mlx->ret |= clSetKernelArg(mlx->kernel, 33, sizeof(cl_int), &mlx->norm_mapping);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("kernel_arg error %d\n", mlx->ret);
		exit(0);
	}

	mlx->ret = clEnqueueNDRangeKernel(mlx->command_queue, mlx->kernel, 1, NULL, &mlx->global_work_size, &mlx->local_work_size, 0, NULL, NULL);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("enqueue error %d\n", mlx->ret);
		exit(0);
	}
	mlx->ret = clFinish(mlx->command_queue);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("cl_finish error %d\n", mlx->ret);
		exit(0);
	}

	mlx->ret = clEnqueueReadBuffer(mlx->command_queue, mlx->buffer, CL_TRUE, 0, sizeof(unsigned int) * (W * H), mlx->data, 0, NULL, NULL);
	if (mlx->ret != CL_SUCCESS)
	{
		printf("read_buffer error %d\n", mlx->ret);
		exit(0);
	}
}
