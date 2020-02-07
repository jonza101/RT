/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 18:09:56 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/17 18:09:57 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			ft_is_white_space(char c)
{
    if (c == 32)
        return (1);
    return (0);
}

void		free_splited(char **splited_str)
{
    int		i;

    i = 0;
    while (splited_str[i])
    {
        ft_strdel(&splited_str[i]);
        i++;
    }
    free(splited_str);
}

void		error(char *str)
{
    ft_putstr(str);
    ft_putchar('\n');
    exit(1);
}

int			count_words(char **str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int	base(int c)
{
    char *str = "0123456789abcdef";
    char *str2 = "0123456789ABCDEF";
    int  i = 0;

    while (i < 16)
    {
        if (c == str[i] || c == str2[i])
            return (i);
        i++;
    }
    return (-1);
}

int ft_atoi_base16(const char *str)
{
    int nb = 0;
    int	i = 0;
    if (str[i] == '0' && ft_strlen(str) > 2 && str[i + 1] == 'x')
        i = i + 2;
    while (base(str[i]) != -1)
    {
        nb = nb * 16;
        nb = nb + base(str[i]);
        i++;
    }
    return (nb);
}

float		ft_atof(char *str)
{
    float	res;
    float	power;
    size_t	i;
    float	sign;

    res = 0.0;
    i = 0;
    power = 1.0;
    while (ft_is_white_space(str[i]))
        i++;
    sign = (str[i] == '-') ? -1 : 1;
    if (str[i] == '+' || str[i] == '-')
        i++;
    i--;
    while (ft_isdigit(str[++i]))
        res = res * 10 + (str[i] - '0');
    if (str[i] == '.')
        i++;
    while (ft_isdigit(str[i]))
    {
        res = res * 10 + (str[i] - '0');
        power *= 10.0;
        i++;
    }
    return (res * sign / power);
}

float		get_color(char *str)
{
    char	**splited_str;
    float color;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 2)
        error("invalid map");
    color = ft_atoi_base16(splited_str[1]);
    free_splited(splited_str);
    return (color);
}

int         get_txt(char *str)
{
    int number;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 2)
        error("invalid map");
    if (ft_strcmp(splited_str[1], "NULL") == 0)
        return (0);
    else
        number = ft_atoi(splited_str[1]);
    free_splited(splited_str);
    return (number);
}

t_vec3	*get_point(char *str)
{
    t_vec3	*point;
    char	**splited_str;

    point = (t_vec3 *)malloc(sizeof(t_vec3));
    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 4)
        error("invalid map");
    point->x = ft_atof(splited_str[1]);
    point->y = ft_atof(splited_str[2]);
    point->z = ft_atof(splited_str[3]);
    free_splited(splited_str);
    return (point);
}

float		get_float(char *str)
{
    float	intensity;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 2)
        error("invalid map");
    intensity = atof(splited_str[1]);
    free_splited(splited_str);
    return (intensity);
}

/*
 * получаем тип источника света
 */
int			get_type_l(char *str)
{
    int		type;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 2)
        error("invalid map");
    if (!(ft_strcmp(splited_str[1], "ambient")))
        type = 0;
    else if (!(ft_strcmp(splited_str[1], "point")))
        type = 1;
    else
        error("invalid type of light");
    free_splited(splited_str);
    return (type);
}

/*
 * инициализируем начальные параметры
 */
t_light *init_light()
{
    t_light *light;
    light = (t_light*)malloc(sizeof(t_light));
    light->vec = (t_vec3*)malloc(sizeof(t_vec3));
    light->intensity = 0.0f;
    light->radius = 1.0f;
    return (light);
}

/*
 * для всех источников света считываем:
 * тип, интенсивность, цвет
 * для точечного еще кординату положения
 */
void		add_light(int fd, t_mlx *mlx, int current_i)
{
    int		type;
    float	intensity;
    t_vec3	*point;
    char	*str;

    mlx->light[current_i] = init_light();
    point = NULL;
    get_next_line(fd, &str);
    type = get_type_l(str);
    mlx->light[current_i]->type = type;
    free(str);
    get_next_line(fd, &str);
    mlx->light[current_i]->intensity = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->light[current_i]->color = get_color(str);
    free(str);
    if (type != 0)
    {
        get_next_line(fd, &str);
        mlx->light[current_i]->vec = get_point(str);
        free(str);
    }
    /*
    printf("light type: %d\n", mlx->light[current_i]->type);
    printf("intensity: %f\n", mlx->light[current_i]->intensity);
    printf("color: %d\n", mlx->light[current_i]->color);
    printf("light center: %f %f %f\n", mlx->light[current_i]->vec->x, mlx->light[current_i]->vec->y, mlx->light[current_i]->vec->z);
    */
    get_next_line(fd, &str);
    if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
}

/*
 * инициализируем начальный объект
 */
t_obj *init_object()
{
    t_obj	*object;

    object = (t_obj*)malloc(sizeof(t_obj));
    object->c = (t_vec3*)malloc(sizeof(t_vec3));
    object->normal = (t_vec3*)malloc(sizeof(t_vec3));
    object->oc_temp = (t_vec3*)malloc(sizeof(t_vec3));
    object->p0 = (t_vec3*)malloc(sizeof(t_vec3));
    object->p1 = (t_vec3*)malloc(sizeof(t_vec3));
    object->p2 = (t_vec3*)malloc(sizeof(t_vec3));
    object->vec_temp = (t_vec3*)malloc(sizeof(t_vec3));
    object->vec_tmp = (t_vec3*)malloc(sizeof(t_vec3));
    object->t_p = (t_vec3*)malloc(sizeof(t_vec3));
    object->n_temp = (t_vec3 *)malloc(sizeof(t_vec3));
    object->tmp = (t_vec3 *)malloc(sizeof(t_vec3));

    object->color = 0x0;
    object->radius = 0.0f;
    object->specular = 0.0f;
    object->mirrored = 0.0f;
    object->transparency = 0.0f;
    object->refractive_index = 1.0f;
    object->txt = NULL;
    object->txt_trans = 0;

    return(object);
}

/*
 * инициализируем начальный объект, добавляем ему тип
 * считываем кординаты центра
 * считываем радиус
 * считываем нормаль, нормализируем
 * считываем цвет, нужно добавить считывание в 16-и ричной
 * считываем прозрачность
 * считываем зеркальность
 * добавляем функции
 *
 * добавить высоту. точка - центр или начало?
 */

void			read_cylinder(t_mlx *mlx, int fd, int type, int current_i)
{
    char		*str;

    mlx->obj[current_i] = init_object();
    mlx->obj[current_i]->type=type;
    get_next_line(fd, &str);
    mlx->obj[current_i]->c = get_point(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->radius = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->normal = get_point(str);
    mlx->obj[current_i]->normal = ft_vec_normalize(mlx->obj[current_i]->normal);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->color = get_color(str);
    free(str);
    get_next_line(fd, &str);
    if (!get_txt(str))
        mlx->obj[current_i]->txt = NULL;
    else
        mlx->obj[current_i]->txt = mlx->txt[get_txt(str)];
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_sph_intersect;
    mlx->obj[current_i]->normal_calc = ft_sph_normal_calc;
    mlx->obj[current_i]->txt_map = ft_sph_txt_map;
}

/*
 * инициализируем начальный объект, добавляем ему тип
 * считываем кординаты центра
 * считываем радиус
 * считываем нормаль, нормализируем
 * считываем цвет, нужно добавить считывание в 16-и ричной
 * считываем прозрачность
 * считываем зеркальность
 * добавляем функции
 *
 * добавить высоту. точка - центр или начало?
 */

void			read_cone(t_mlx *mlx, int fd, int type, int current_i)
{
    char		*str;

    mlx->obj[current_i] = init_object();
    mlx->obj[current_i]->type=type;
    get_next_line(fd, &str);
    mlx->obj[current_i]->c = get_point(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->radius = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->normal = get_point(str);
    mlx->obj[current_i]->normal = ft_vec_normalize(mlx->obj[current_i]->normal);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->color = get_color(str);
    free(str);
    get_next_line(fd, &str);
    if (!get_txt(str))
        mlx->obj[current_i]->txt = NULL;
    else
        mlx->obj[current_i]->txt = mlx->txt[get_txt(str)];
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_sph_intersect;
    mlx->obj[current_i]->normal_calc = ft_sph_normal_calc;
    mlx->obj[current_i]->txt_map = ft_sph_txt_map;
}

/*
 * инициализируем начальный объект, добавляем ему тип
 * считываем кординаты центра
 * считываем радиус
 * считываем нормаль, нормализируем
 * считываем цвет, нужно добавить считывание в 16-и ричной
 * считываем прозрачность
 * считываем зеркальность
 * добавляем функции
 */

void			read_sphere(t_mlx *mlx, int fd, int type, int current_i)
{
    char		*str;

    mlx->obj[current_i] = init_object();
    mlx->obj[current_i]->type=type;
    get_next_line(fd, &str);
    mlx->obj[current_i]->c = get_point(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->radius = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->normal = get_point(str);
    mlx->obj[current_i]->normal = ft_vec_normalize(mlx->obj[current_i]->normal);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->color = get_color(str);
    free(str);
    get_next_line(fd, &str);
    if (!get_txt(str))
        mlx->obj[current_i]->txt = NULL;
    else
        mlx->obj[current_i]->txt = mlx->txt[get_txt(str)];
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_sph_intersect;
    mlx->obj[current_i]->normal_calc = ft_sph_normal_calc;
    mlx->obj[current_i]->txt_map = ft_sph_txt_map;
    /*
    printf("type: object\n");
    printf("subtype: %d\n", mlx->obj[current_i]->type);
    printf("center: %f %f %f\n", mlx->obj[current_i]->c->x, mlx->obj[current_i]->c->y, mlx->obj[current_i]->c->z);
    printf("radius: %f\n", mlx->obj[current_i]->radius);
    printf("normal: %f %f %f\n", mlx->obj[current_i]->normal->x, mlx->obj[current_i]->normal->y, mlx->obj[current_i]->normal->z);
    printf("color: %d\n", mlx->obj[current_i]->color);
    if (mlx->obj[current_i]->txt)
        printf("txt - yes\n");
    else
        printf("txt - NULL\n");
    printf("specular: %f\n", mlx->obj[current_i]->specular);
    printf("mirrored: %f\n", mlx->obj[current_i]->mirrored);
    */
}


/*
 * инициализируем начальный объект, добавляем ему тип
 * считываем кординаты точки
 * считываем нормаль, нормализируем
 * считываем цвет, нужно добавить считывание в 16-и ричной
 * считываем прозрачность
 * считываем зеркальность
 * добавляем функции
 */
void			read_plane(t_mlx *mlx, int fd, int type, int current_i)
{
    char		*str;

    mlx->obj[current_i] = init_object();
    mlx->obj[current_i]->type=type;
    get_next_line(fd, &str);
    mlx->obj[current_i]->c = get_point(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->normal = get_point(str);
    mlx->obj[current_i]->normal = ft_vec_normalize(mlx->obj[current_i]->normal);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->color = get_color(str);
    free(str);
    get_next_line(fd, &str);
    if (!get_txt(str)) {
        mlx->obj[current_i]->txt = NULL;
    }
    else {
        mlx->obj[current_i]->txt = mlx->txt[get_txt(str)];
    }
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_plane_intersect;
    mlx->obj[current_i]->normal_calc = ft_plane_normal_calc;
    mlx->obj[current_i]->txt_map = ft_plane_txt_map;
}

/*
 * получение типа объекта по первой строке
 */
int			get_type_o(char *str)
{
    int		type;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 2)
        error("invalid map");
    if (!(ft_strcmp(splited_str[1], "sphere")))
        type = 0;
    else if (!(ft_strcmp(splited_str[1], "plane")))
        type = 1;
    else if (!(ft_strcmp(splited_str[1], "cone")))
        type = 2;
    else if (!(ft_strcmp(splited_str[1], "cylinder")))
        type = 3;
    else
        error("invalid type of object");
    free_splited(splited_str);
    return (type);
}

/*
 * сначала получаем тип объекта
 * в зависимости от типа вызываем соответствующую функцию
 */
void		add_object(int fd, t_mlx *mlx, int current_i)
{
    char	*str;
    int		type;

    get_next_line(fd, &str);
    type = get_type_o(str);
    free(str);
    if (type == 0)
        read_sphere(mlx, fd, type, current_i);
    else if (type == 1)
        read_plane(mlx, fd, type, current_i);
    else if (type == 2)
        read_cone(mlx, fd, type, current_i);
    else if (type == 3)
        read_cylinder(mlx, fd, type, current_i);
    else
        error ("invalid type");
    get_next_line(fd, &str);
    if ((ft_strcmp(str, "}\n")) && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
}


/*
 * инициализируем объект mlx, в который записываем
 * имеющееся число объектов и источников света
 */
void    init_mlx_to_zero(int n_objects, int n_lights, t_mlx *mlx)
{
    mlx->cam = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->cam->x = 0.0f;
    mlx->cam->y = 0.0f;
    mlx->cam->z = 0.0f;

    mlx->dx = 0.0f;
    mlx->dy = 0.0f;

    mlx->oc = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->dir = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->point = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->normal = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->light_dir = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->neg_dir = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->s_refl = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->refl_ray = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->refr_ray = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->s_dir = (t_vec3*)malloc(sizeof(t_vec3));
    mlx->obj_count = n_objects;
    mlx->obj = (t_obj**)malloc(sizeof(t_obj*) * mlx->obj_count);
    mlx->light_count = n_lights;
    mlx->light = (t_light**)malloc(sizeof(t_light*) * mlx->light_count);

    i = -1;
    while (++i < 4)
    {
        mlx->wsad[i] = 0;
        mlx->arrow[i] = 0;
    }
    mlx->shift = 0;
    mlx->last_time = 0.0f;
    mlx->gpu = 1;
    mlx->render_func = ft_execute_kernel;
    mlx->render_device = GPU_STR;
    mlx->cel_band = 1;
    mlx->effect_str[0] = EFFECT_NONE_STR;
    mlx->effect_str[1] = EFFECT_CEL_SHADING_STR;
    mlx->effect_str[2] = EFFECT_SEPIA_STR;
    mlx->effect_str[3] = EFFECT_GRAYSCALE_STR;
    mlx->effect_str[4] = EFFECT_BLACK_WHITE_STR;
    mlx->curr_effect = mlx->effect_str[0];
    mlx->effect_i = 0;
    mlx->negative = 0;
    mlx->negative_str[0] = NEGATIVE_OFF_STR;
    mlx->negative_str[1] = NEGATIVE_ON_STR;
    mlx->noise = 0;
    mlx->noise_str[0] = NOISE_OFF_STR;
    mlx->noise_str[1] = NOISE_ON_STR;
    mlx->colored_light = 0;
    mlx->colored_light_str[0] = COLORED_LIGHT_OFF_STR;
    mlx->colored_light_str[1] = COLORED_LIGHT_ON_STR;
    mlx->soft_shadows = 0;
    mlx->ss_cell = 16;
    mlx->soft_sh_str[0] = SOFT_SHADOWS_OFF_STR;
    mlx->soft_sh_str[1] = SOFT_SHADOWS_ON_STR;
    mlx->bw_factor = 0;
    mlx->ns_factor = 16;
}

/*
 * считывание числа объектов и источников света
 * проверка строки на "новый объект" или "новый свет"
 * вызов функции добавления с передачей ей текущего индекса для объекта/света
 */
t_mlx		*read_file(int fd, t_mlx *mlx)
{
    char	*str;
    int current_obj;
    int current_light;

    current_obj = 0;
    current_light = 0;
    while (get_next_line(fd, &(str)))
    {
        if (ft_strcmp(str, "light {") == 0)
        {
            add_light(fd, mlx, current_light);
            current_light++;
            printf("light added\n");
        }
        else if (ft_strcmp(str, "object {") == 0)
        {
            add_object(fd, mlx, current_obj);
            current_obj++;
            printf("object added\n");
        }
        else
            error("invalid map");
    }
    if (current_light != mlx->light_count || current_obj != mlx->obj_count)
        error("invalid map");
    return (mlx);
}

/*
 * parser заменяет функцию void	ft_init(t_mlx *mlx)
 * в мейне необходимо прописать
 *
    int fd;
    t_mlx *mlx;
    fd = open(argv[1], O_RDONLY);
    mlx = (t_mlx*)malloc(sizeof(t_mlx));
    mlx->mlx = mlx_init();

    parser(mlx, fd);
 * в начале инициализируем txt
 * после этого считываем число объектов и источников света
 * вызываем функцию инициализации
 * вызываем функцию считывания
 */
void    parser(t_mlx *mlx, int fd)
{
    char	*str;
    int     n_objects;
    int     n_lights;

    srand(time(NULL));
    ft_init_txt(mlx);

    get_next_line(fd, &(str));
    if (ft_strncmp(str, "objects count: ", 15) == 0)
        n_objects = ft_atoi(&str[15]);
    get_next_line(fd, &(str));
    if (ft_strncmp(str, "lights count: ", 14) == 0)
        n_lights = ft_atoi(&str[14]);
    init_mlx_to_zero(n_objects, n_lights, mlx);
    read_file(fd, mlx);
    ft_init_gpu(mlx);
    ft_load_cl_files(mlx);
}