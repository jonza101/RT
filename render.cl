#define W 1920
#define H 1080
#define AR ((double)W / (double)H)

#define BACKGROUND_COLOR 0x0

#define SPHERE 0
#define PLANE 1
#define CONE 2
#define CYLINDER 3

#define AMBIENT_L 0
#define POINT_L 1

#define OBJ 0
#define LIGHT 0

typedef struct		s_vec3
{
	double			x;
	double			y;
	double			z;
}                  		 t_vec3;

typedef	struct		s_light				//		0 - AMBIENT		|	1 - POINT	|	2 - DIRECTIONAL		|
{
	int					type;
	double			intensity;
	t_vec3			vec;
}						t_light;

typedef	struct		s_obj
{
	int					type;
	t_vec3			c;
	double			radius;
	t_vec3			normal;

	int					color;
	double			specular;
	double           mirrored;
}						t_obj;

typedef struct		s_scene
{
	t_obj			obj[OBJ];
	t_light			light[LIGHT];
}							t_scene;


int		ft_trace_ray(double origin_x, double origin_y, double origin_z,
							double dir_x, double dir_y, double dir_z,
							__global double *obj_x, __global double *obj_y, __global double *obj_z,
							__global double *obj_norm_x, __global double *obj_norm_y, __global double *obj_norm_z,
							__global double *obj_radius, __global int *obj_color, __global double *obj_specular,
							__global double *obj_mirrored, int obj_count, __global int *obj_type,
							__global double *light_x, __global double *light_y, __global double *light_z,
							__global int *light_type, __global double *light_intensity, int light_count,
							double min_dist, double max_dist, int depth);


double ft_dot_prod(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return (x1 * x2 + y1 * y2 + z1 * z2);
}

double		ft_vec_len(double x, double y, double z)
{
	double len = sqrt(x * x + y * y + z * z);
	return (len);
}

void		ft_vec_normalize(double *x, double *y, double *z)
{
	double len = ft_vec_len(*x, *y, *z);
	*x = (double)(*x) / (double)len;
	*y = (double)(*y) / (double)len;
	*z = (double)(*z) / (double)len;
}

int			ft_color_convert(int color, double lum)
{
	int r = ((color >> 16) & 0xFF) * lum;
	int g = ((color >> 8) & 0xFF) * lum;
	int b = (color & 0xFF) * lum;

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int         ft_sum_color(int c1, int c2, double k1, double k2)
{
    int r1 = (c1 >> 16) & 0xFF;
    int g1 = (c1 >> 8) & 0xFF;
    int b1 = c1 & 0xFF;
    int r2 = (c2 >> 16) & 0xFF;
    int g2 = (c2 >> 8) & 0xFF;
    int b2 = c2 & 0xFF;
    int r = r1 * k1 + r2 * k2;
    int g = g1 * k1 + g2 * k2;
    int b = b1 * k1 + b2 * k2;
    return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

//////////////////////////////////////////////////////////////////////	INTERSECT	//////////////////////////////////////////////////////////////////////////////
double		ft_sph_intersect(double origin_x, double origin_y, double origin_z,
										double dir_x, double dir_y, double dir_z,
										double obj_x, double obj_y, double obj_z,
										double obj_radius)
{
	double oc_x = origin_x - obj_x;
	double oc_y = origin_y - obj_y;
	double oc_z = origin_z - obj_z;

	double k1 = ft_dot_prod(dir_x, dir_y, dir_z, dir_x, dir_y, dir_z);
	double k2 = 2.0f * ft_dot_prod(oc_x, oc_y, oc_z, dir_x, dir_y, dir_z);
	double k3 = ft_dot_prod(oc_x, oc_y, oc_z, oc_x, oc_y, oc_z) - (obj_radius * obj_radius);

	double discr = k2 * k2 - (4.0f * k1 * k3);
	if (discr < 0.0f)
		return (-1.0f);
	
	double discr_sqrt = sqrt(discr);
	double k1_ = 2.0f * k1;

	double t1 = (double)(-k2 + discr_sqrt) / (double)k1_;
	double t2 = (double)(-k2 - discr_sqrt) / (double)k1_;
	if (t1 < t2)
		return (t1);
	return (t2);
}

double		ft_plane_intersect(double origin_x, double origin_y, double origin_z,
											double dir_x, double dir_y, double dir_z,
											double obj_x, double obj_y, double obj_z,
											double obj_norm_x, double obj_norm_y, double obj_norm_z)
{
	double denom = ft_dot_prod(obj_norm_x, obj_norm_y, obj_norm_z, dir_x, dir_y, dir_z);
	double denom_abs = denom;
	if (denom < 0.0f)
		denom_abs = -denom;
	if (denom_abs > 0.000001f)
	{
		double oc_x = obj_x - origin_x;
		double oc_y = obj_y - origin_y;
		double oc_z = obj_z - origin_z;

		double t = (double)ft_dot_prod(oc_x, oc_y, oc_z, obj_norm_x, obj_norm_y, obj_norm_z) / (double)denom;
		if (t > 0.000001f)
			return (t);
	}
	return (-1.0f);
}

double		ft_cone_intersect(double origin_x, double origin_y, double origin_z,
										double dir_x, double dir_y, double dir_z,
										double obj_x, double obj_y, double obj_z,
										double obj_norm_x, double obj_norm_y, double obj_norm_z,
										double obj_radius)
{
	double oc_x = origin_x - obj_x;
	double oc_y = origin_y - obj_y;
	double oc_z = origin_z - obj_z;

	ft_vec_normalize(&obj_norm_x, &obj_norm_y, &obj_norm_z);

	double pow_ = pow(tan(obj_radius), 2);

	double oc_dot_n = ft_dot_prod(oc_x, oc_y, oc_z, obj_norm_x, obj_norm_y, obj_norm_z);
	double d_dot_n = ft_dot_prod(dir_x, dir_y, dir_z, obj_norm_x, obj_norm_y, obj_norm_z);

	double k1 = ft_dot_prod(dir_x, dir_y, dir_z, dir_x, dir_y, dir_z) - (1.0f + pow_) * pow(d_dot_n, 2);
	double k2 = 2.0f * (ft_dot_prod(dir_x, dir_y, dir_z, oc_x, oc_y, oc_z) - (1.0f + pow_) * d_dot_n * oc_dot_n);
	double k3 = ft_dot_prod(oc_x, oc_y, oc_z, oc_x, oc_y, oc_z) - (1.0f + pow_) * pow(oc_dot_n, 2);

	double discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (-1.0f);

	double discr_sqrt = sqrt(discr);
	double k1_ = 2.0f * k1;
	double t1 = (double)(-k2 + discr_sqrt) / (double)k1_;
	double t2 = (double)(-k2 - discr_sqrt) / (double)k1_;

	if (t1 < t2)
		return (t1);
	return (t2);
}

double		ft_cylinder_intersect(double origin_x, double origin_y, double origin_z,
											double dir_x, double dir_y, double dir_z,
											double obj_x, double obj_y, double obj_z,
											double obj_norm_x, double obj_norm_y, double obj_norm_z,
											double obj_radius)
{
	double oc_x = origin_x - obj_x;
	double oc_y = origin_y - obj_y;
	double oc_z = origin_z - obj_z;

	ft_vec_normalize(&obj_norm_x, &obj_norm_y, &obj_norm_z);

	double d_dot_n = ft_dot_prod(dir_x, dir_y, dir_z, obj_norm_x, obj_norm_y, obj_norm_z);
	double oc_dot_n = ft_dot_prod(oc_x, oc_y, oc_z, obj_norm_x, obj_norm_y, obj_norm_z);

	double k1 = ft_dot_prod(dir_x, dir_y, dir_z, dir_x, dir_y, dir_z) - pow(d_dot_n, 2);
	double k2 = 2.0f * (ft_dot_prod(dir_x, dir_y, dir_z, oc_x, oc_y, oc_z) - (d_dot_n * oc_dot_n));
	double k3 = ft_dot_prod(oc_x, oc_y, oc_z, oc_x, oc_y, oc_z) - pow(oc_dot_n, 2) - pow(obj_radius, 2);

	double discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (-1.0f);

	double discr_sqrt = sqrt(discr);
	double k1_ = 2.0f * k1;

	double t1 = (double)(-k2 + discr_sqrt) / (double)k1_;
	double t2 = (double)(-k2 - discr_sqrt) / (double)k1_;

	if (t1 < t2)
		return (t1);
	return (t2);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////	NORMAL		///////////////////////////////////////////////////////////////////////////
void	ft_sph_normal_calc(double dir_x, double dir_y, double dir_z,
										double point_x, double point_y, double point_z,
										double obj_x, double obj_y, double obj_z,
										double *normal_x, double *normal_y, double *normal_z)
{
	*normal_x = point_x - obj_x;
	*normal_y = point_y - obj_y;
	*normal_z = point_z - obj_z;
	ft_vec_normalize(normal_x, normal_y, normal_z);
}

void	ft_plane_normal_calc(double dir_x, double dir_y, double dir_z,
											double obj_norm_x, double obj_norm_y, double obj_norm_z,
											double *normal_x, double *normal_y, double *normal_z)
{
	if (ft_dot_prod(dir_x, dir_y, dir_z, obj_norm_x, obj_norm_y, obj_norm_z) > 0.0f)
	{
		*normal_x = -obj_norm_x;
		*normal_y = -obj_norm_y;
		*normal_z = -obj_norm_z;
	}
	else
	{
		*normal_x = obj_norm_x;
		*normal_y = obj_norm_y;
		*normal_z = obj_norm_z;
	}
}

void		ft_cone_normal_calc(double dir_x, double dir_y, double dir_z,
											double point_x, double point_y, double point_z,
											double obj_x, double obj_y, double obj_z,
											double obj_norm_x, double obj_norm_y, double obj_norm_z,
											double obj_radius,
											double *normal_x, double *normal_y, double *normal_z)
{
	*normal_x = point_x - obj_x;
	*normal_y = point_y - obj_y;
	*normal_z = point_z - obj_z;

	double side_len = (double)sqrt(ft_dot_prod(*normal_x, *normal_y, *normal_z, *normal_x, *normal_y, *normal_z)) / (double)cos(obj_radius);

	if (ft_dot_prod(*normal_x, *normal_y, *normal_z, obj_norm_x, obj_norm_y, obj_norm_z) < 0.0f)
		side_len = -side_len;

	*normal_x = obj_x + obj_norm_x * side_len;
	*normal_y = obj_y + obj_norm_y * side_len;
	*normal_z = obj_z + obj_norm_z * side_len;

	*normal_x = point_x - *normal_x;
	*normal_y = point_y - *normal_y;
	*normal_z = point_z - *normal_z;
	ft_vec_normalize(normal_x, normal_y, normal_z);
}

void		ft_cylinder_normal_calc(double dir_x, double dir_y, double dir_z,
												double point_x, double point_y, double point_z,
												double obj_x, double obj_y, double obj_z,
												double obj_norm_x, double obj_norm_y, double obj_norm_z,
												double *normal_x, double *normal_y, double *normal_z)
{
	*normal_x = point_x - obj_x;
	*normal_y = point_y - obj_y;
	*normal_z = point_z - obj_z;

	double dis = ft_dot_prod(*normal_x, *normal_y, *normal_z, obj_norm_x, obj_norm_y, obj_norm_z);

	*normal_x = obj_x + obj_norm_x * dis;
	*normal_y = obj_y + obj_norm_y * dis;
	*normal_z = obj_z + obj_norm_z * dis;

	*normal_x = point_x - *normal_x;
	*normal_y = point_y - *normal_y;
	*normal_z = point_z - *normal_z;
	ft_vec_normalize(normal_x, normal_y, normal_z);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int		ft_shadow_intersection(double origin_x, double origin_y, double origin_z,
											double dir_x, double dir_y, double dir_z,
											__global double *obj_x, __global double *obj_y, __global double *obj_z,
											__global double *obj_norm_x, __global double *obj_norm_y, __global double *obj_norm_z,
											__global double *obj_radius, int obj_count, __global int *obj_type,
											double min_dist, double max_dist)
{
	int obj_i = -1;
	double closest = max_dist;
	int i = -1;
	while (++i < obj_count)
	{
		//double t = ft_sph_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_radius[i]);
		double t;

		if (obj_type[i] == SPHERE)
			t = ft_sph_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_radius[i]);
		else if (obj_type[i] == PLANE)
			t = ft_plane_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_norm_x[i], obj_norm_y[i], obj_norm_z[i]);
		else if (obj_type[i] == CONE)
			t = ft_cone_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_norm_x[i], obj_norm_y[i], obj_norm_z[i], obj_radius[i]);
		else if (obj_type[i] == CYLINDER)
			t = ft_cylinder_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_norm_x[i], obj_norm_y[i], obj_norm_z[i], obj_radius[i]);

		if (t >= min_dist && t < closest)
		{
			obj_i = i;
			closest = t;
			return (obj_i);
		}
	}
	return (obj_i);
}

int		ft_closest_intersection(double origin_x, double origin_y, double origin_z,
											double dir_x, double dir_y, double dir_z,
											__global double *obj_x, __global double *obj_y, __global double *obj_z,
											__global double *obj_norm_x, __global double *obj_norm_y, __global double *obj_norm_z,
											__global double *obj_radius, int obj_count, __global int *obj_type,
											double min_dist, double max_dist, double *closest)
{
	int obj_i = -1;
	*closest = max_dist;
	int i = -1;
	while (++i < obj_count)
	{
		//double t = ft_sph_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_radius[i]);
		double t;

		if (obj_type[i] == SPHERE)
			t = ft_sph_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_radius[i]);
		else if (obj_type[i] == PLANE)
			t = ft_plane_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_norm_x[i], obj_norm_y[i], obj_norm_z[i]);
		else if (obj_type[i] == CONE)
			t = ft_cone_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_norm_x[i], obj_norm_y[i], obj_norm_z[i], obj_radius[i]);
		else if (obj_type[i] == CYLINDER)
			t = ft_cylinder_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_norm_x[i], obj_norm_y[i], obj_norm_z[i], obj_radius[i]);

		if (t >= min_dist && t < *closest)
		{
			obj_i = i;
			*closest = t;
		}
	}
	return (obj_i);
}

int		ft_trace_refl_ray(double origin_x, double origin_y, double origin_z,
								double dir_x, double dir_y, double dir_z,
								__global double *obj_x, __global double *obj_y, __global double *obj_z,
								__global double *obj_norm_x, __global double *obj_norm_y, __global double *obj_norm_z,
								__global double *obj_radius, __global int *obj_color, __global double *obj_specular,
								__global double *obj_mirrored, int obj_count, __global int *obj_type,
								__global double *light_x, __global double *light_y, __global double *light_z,
								__global int *light_type, __global double *light_intensity, int light_count,
								double min_dist, double max_dist, int depth)
{
	int color = ft_trace_ray(	origin_x, origin_y, origin_z,
										dir_x, dir_y, dir_z,
										obj_x, obj_y, obj_z,
										obj_norm_x, obj_norm_y, obj_norm_z,
										obj_radius, obj_color, obj_specular,
										obj_mirrored, obj_count, obj_type,
										light_x, light_y, light_z,
										light_type, light_intensity, light_count,
										min_dist, max_dist, depth);
	return (color);
}

int		ft_trace_ray(double origin_x, double origin_y, double origin_z,
							double dir_x, double dir_y, double dir_z,
							__global double *obj_x, __global double *obj_y, __global double *obj_z,
							__global double *obj_norm_x, __global double *obj_norm_y, __global double *obj_norm_z,
							__global double *obj_radius, __global int *obj_color, __global double *obj_specular,
							__global double *obj_mirrored, int obj_count, __global int *obj_type,
							__global double *light_x, __global double *light_y, __global double *light_z,
							__global int *light_type, __global double *light_intensity, int light_count,
							double min_dist, double max_dist, int depth)
{
	double closest;
	int obj_i = ft_closest_intersection(origin_x, origin_y, origin_z,
													dir_x, dir_y, dir_z,
													obj_x, obj_y, obj_z,
													obj_norm_x, obj_norm_y, obj_norm_z,
													obj_radius, obj_count, obj_type,
													min_dist, max_dist, &closest);
	if (obj_i < 0)
		return (BACKGROUND_COLOR);
	//return (obj_color[obj_i]);

	double point_x = origin_x + closest * dir_x;
	double point_y = origin_y + closest * dir_y;
	double point_z = origin_z + closest * dir_z;

	double normal_x;
	double normal_y;
	double normal_z;

	if (obj_type[obj_i] == SPHERE)
		ft_sph_normal_calc(dir_x, dir_y, dir_z, point_x, point_y, point_z, obj_x[obj_i], obj_y[obj_i], obj_z[obj_i], &normal_x, &normal_y, &normal_z);
	else if (obj_type[obj_i] == PLANE)
		ft_plane_normal_calc(dir_x, dir_y, dir_z, obj_norm_x[obj_i], obj_norm_y[obj_i], obj_norm_z[obj_i], &normal_x, &normal_y, &normal_z);
	else if (obj_type[obj_i] == CONE)
		ft_cone_normal_calc(dir_x, dir_y, dir_z, point_x, point_y, point_z, obj_x[obj_i], obj_y[obj_i], obj_z[obj_i],
										obj_norm_x[obj_i], obj_norm_y[obj_i], obj_norm_z[obj_i], obj_radius[obj_i],
										&normal_x, &normal_y, &normal_z);
	else if (obj_type[obj_i] == CYLINDER)
		ft_cylinder_normal_calc(dir_x, dir_y, dir_z, point_x, point_y, point_z, obj_x[obj_i], obj_y[obj_i], obj_z[obj_i], obj_norm_x[obj_i], obj_norm_y[obj_i], obj_norm_z[obj_i],
											&normal_x, &normal_y, &normal_z);

	double light_dir_x;
	double light_dir_y;
	double light_dir_z;
	double s_max;

	double intensity = 0.0f;
	int i = -1;
	while (++i < light_count)
	{
		if (light_type[i] == AMBIENT_L)
			intensity += light_intensity[i];
		else
		{
			if (light_type[i] == POINT_L)
			{
				light_dir_x = light_x[i] - point_x;
				light_dir_y = light_y[i] - point_y;
				light_dir_z = light_z[i] - point_z;
				s_max = 1.0f;
			}
			else
				continue;

			double s_closest;
			int s_obj_i = ft_shadow_intersection(point_x, point_y, point_z,
																light_dir_x, light_dir_y, light_dir_z,
																obj_x, obj_y, obj_z,
																obj_norm_x, obj_norm_y, obj_norm_z,
																obj_radius, obj_count, obj_type,
																0.000001f, s_max);
			if (s_obj_i >= 0)
				continue;

			double n_dot_l = ft_dot_prod(normal_x, normal_y, normal_z, light_dir_x, light_dir_y, light_dir_z);
			if (n_dot_l > 0.0f)
				intensity += (double)(light_intensity[i] * n_dot_l) / (double)(ft_vec_len(normal_x, normal_y, normal_z) * ft_vec_len(light_dir_x, light_dir_y, light_dir_z));

			if (obj_specular[obj_i] > 0.0f)
			{
				double dot = ft_dot_prod(normal_x, normal_y, normal_z, light_dir_x, light_dir_y, light_dir_z);
				double s_refl_x = 2.0f * normal_x * dot - light_dir_x;
				double s_refl_y = 2.0f * normal_y * dot - light_dir_y;
				double s_refl_z = 2.0f * normal_z * dot - light_dir_z;

				double r_dot_v = ft_dot_prod(s_refl_x, s_refl_y, s_refl_z, -dir_x, -dir_y, -dir_z);
				if (r_dot_v > 0.0f)
					intensity += (light_intensity[i] * pow((double)r_dot_v / (double)(ft_vec_len(s_refl_x, s_refl_y, s_refl_z) * ft_vec_len(-dir_x, -dir_y, -dir_z)), (int)obj_specular[obj_i]));
			}
		}
	}

	if (intensity > 1.0f)
		intensity = 1.0f;

	int color = ft_color_convert(obj_color[obj_i], intensity);
	if (depth > 1 || obj_mirrored[obj_i] <= 0)
		return (color);

	double refl_dot = ft_dot_prod(-dir_x, -dir_y, -dir_z, normal_x, normal_y, normal_z);
	double refl_x = 2.0f * refl_dot * normal_x - (-dir_x);
	double refl_y = 2.0f * refl_dot * normal_y - (-dir_y);
	double refl_z = 2.0f * refl_dot * normal_z - (-dir_z);

	int refl_color = ft_trace_refl_ray(point_x, point_y, point_z,
												refl_x, refl_y, refl_z,
												obj_x, obj_y, obj_z,
												obj_norm_x, obj_norm_y, obj_norm_z,
												obj_radius, obj_color, obj_specular,
												obj_mirrored, obj_count, obj_type,
												light_x, light_y, light_z,
												light_type, light_intensity, light_count,
												0.000001f, 3.40282347e+38F, depth + 1);

	return (ft_sum_color(color, refl_color, 1 - obj_mirrored[obj_i], obj_mirrored[obj_i]));
}

__kernel void render(__global unsigned int *buffer,
							double origin_x, double origin_y, double origin_z,
							__global double *obj_x, __global double *obj_y, __global double *obj_z,
							__global double *obj_norm_x, __global double *obj_norm_y, __global double *obj_norm_z,
							__global double *obj_radius, __global int *obj_color, __global double *obj_specular,
							__global double *obj_mirrored, int obj_count, __global int *obj_type,
							__global double *light_x, __global double *light_y, __global double *light_z,
							__global double *light_type, __global double *light_intensity, int light_count,
							double dx, double dy)
{
	unsigned int pixel = get_global_id(0);

	int y = pixel / W - (H / 2.0f);
	int x = pixel % W - (W / 2.0f);

	double dir_x = (double)x / (double)W * (double)AR + dx;
	double dir_y = -(double)y / (double)H + dy;
	double dir_z = 1.0f;

	int color = ft_trace_ray(origin_x, origin_y, origin_z,
										dir_x, dir_y, dir_z,
										obj_x, obj_y, obj_z,
										obj_norm_x, obj_norm_y, obj_norm_z,
										obj_radius, obj_color, obj_specular,
										obj_mirrored, obj_count, obj_type,
										light_x, light_y, light_z,
										light_type, light_intensity, light_count,
										1.0f, 3.40282347e+38F, 0);

	buffer[pixel] = color;
}