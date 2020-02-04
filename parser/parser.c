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

float		get_color(char *str)
{
    char	**splited_str;
    float color;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 2)
        error("invalid map");
    color = ft_atoibase(splited_str[2], 16);
    free_splited(splited_str);
    return (color)
}

t_point		*get_point(char *str)
{
    t_point	*point;
    char	**splited_str;

    point = (t_point *)malloc(sizeof(t_point));
    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 5)
        error("invalid map");
    point->x = ft_atof(splited_str[2]);
    point->y = ft_atof(splited_str[3]);
    point->z = ft_atof(splited_str[4]);
    free_splited(splited_str);
    return (point);
}

float		get_float(char *str)
{
    float	intensity;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
        error("invalid map");
    intensity = atof(splited_str[2]);
    free_splited(splited_str);
    return (intensity);
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

int			ft_isspace(char c)
{
    if (c == 32)
        return (1);
    return (0);
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
    while (ft_isspace(str[i]))
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

/*
 * получаем тип источника света
 */
int			get_type_l(char *str)
{
    int		type;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
        error("invalid map");
    if (!(ft_strcmp(splited_str[2], "ambient")))
        type = 0;
    else if (!(ft_strcmp(splited_str[2], "point")))
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
    t_point	*point;
    char	*str;

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
    get_next_line(fd, &str);
    if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
}

/*
 * инициализируем начальный объект
 */
t_object *init_object()
{
    t_object	*object;

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
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_sph_intersect;
    mlx->obj[current_i]->normal_calc = ft_sph_normal_calc;
    mlx->obj[current_i]->txt_map = ft_sph_txt_map;
    get_next_line(fd, &str);
    if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
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
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_sph_intersect;
    mlx->obj[current_i]->normal_calc = ft_sph_normal_calc;
    mlx->obj[current_i]->txt_map = ft_sph_txt_map;
    get_next_line(fd, &str);
    if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
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
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_sph_intersect;
    mlx->obj[current_i]->normal_calc = ft_sph_normal_calc;
    mlx->obj[current_i]->txt_map = ft_sph_txt_map;
    get_next_line(fd, &str);
    if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
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
    mlx->obj[current_i]->specular = get_float(str);
    free(str);
    get_next_line(fd, &str);
    mlx->obj[current_i]->mirrored = get_float(str);
    free(str);
    mlx->obj[current_i]->intersect = ft_plane_intersect;
    mlx->obj[current_i]->normal_calc = ft_plane_intersect;
    mlx->obj[current_i]->txt_map = ft_plane_intersect;
    get_next_line(fd, &str);
    if (ft_strcmp(str, "}\n") && ft_strcmp(str, "}"))
        error("invalid map");
    free(str);
}

/*
 * получение типа объекта по первой строке
 */
int			get_type_o(char *str)
{
    int		type;
    char	**splited_str;

    if (!(splited_str = ft_strsplit(str, ' ')) || count_words(splited_str) != 3)
        error("invalid map");
    if (!(ft_strcmp(splited_str[2], "sphere")))
        type = 0;
    else if (!(ft_strcmp(splited_str[2], "plane")))
        type = 1;
    else if (!(ft_strcmp(splited_str[2], "cone")))
        type = 2;
    else if (!(ft_strcmp(splited_str[2], "cylinder")))
        type = 3;
    else
        error("invalid type of light");
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
        read_plane(mlx, fd, type);
    else if (type == 1)
        read_sphere(mlx, fd, type, current_i);
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
}

/*
 * считывание числа объектов и источников света
 * проверка строки на "новый объект" или "новый свет"
 * вызов функции добавления с передачей ей текущего индекса для объекта/света
 */
t_mlx		*read_file(int fd)
{
    char	*str;
    t_mlx	*mlx;
    int current_obj;
    int current_light;

    mlx = init_mlx_to_zero();
    current_obj = 0;
    current_light = 0;
    while (get_next_line(fd, &(str)))
    {
        if (ft_strcmp(str, "light {") == 0)
        {
            add_light(fd, mlx, current_light);
            current_light++;
        }
        else if (ft_strcmp(str, "object {") == 0)
        {
            add_object(fd, mlx, current_obj);
            current_obj++;
        }
        else
            error("invalid map");
    }
    return (mlx);
}

/*
 * в начале инициализируем txt ?
 * после этого считываем число объектов и источников света
 * вызываем функцию инициализации
 * вызываем функцию считывания
 */
void    parser(t_mlx *mlx, int fd)
{
    char	*str;

    srand(time(NULL));
    ft_init_txt(mlx);

    get_next_line(fd, &(str));
    if (ft_strncmp(str, "objects count: ", 15) == 0)
        n_objects = ft_atoi(str);
    get_next_line(fd, &(str));
    if (ft_strncmp(str, "lights count: ", 14) == 0)
        n_lights = ft_atoi(str);
    init_mlx_to_zero(n_objects, n_lights, mlx);
    read_file(fd, mlx)
}