#define W 1920
#define H 1080
#define AR ((float)W / (float)H)

#define BACKGROUND_COLOR 0x0

#define SPHERE 0
#define PLANE 1
#define CONE 2
#define CYLINDER 3

#define AMBIENT_L 0
#define POINT_L 1

#define MAX_FLT 3.40282346638528859811704183484516925e+38F


int		ft_trace_ray(float3 origin, float3 dir,
					__global float3 *obj_pos,
					__global float3 *obj_normal,
					__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
					__global float *obj_mirrored, int obj_count, __global int *obj_type,
					__global float3 *light_vec,
					__global int *light_type, __global float *light_intensity, int light_count,
					float min_dist, float max_dist, int depth, int refl_i);


float ft_dot_prod(float3 a, float3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float		ft_vec_len(float3 vec)
{
	float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return (len);
}

float3		ft_vec_normalize(float3 vec)
{
	float len = ft_vec_len(vec);
	vec.x = (float)(vec.x) / (float)len;
	vec.y = (float)(vec.y) / (float)len;
	vec.z = (float)(vec.z) / (float)len;
	return (vec);
}

float3	ft_rotate_x(float3 vec, float angle)
{
	float cos_ang = cos(angle);
	float sin_ang = sin(angle);
	
	float3 temp = vec;

	vec.x = vec.x;
	vec.y = (temp.y * cos_ang) - (temp.z * sin_ang);
	vec.z = (temp.y * sin_ang) + (temp.z * cos_ang);
	return (vec);
}

float3	ft_rotate_y(float3 vec, float angle)
{
	float cos_ang = cos(angle);
	float sin_ang = sin(angle);
	
	float3 temp = vec;

	vec.x = (temp.x * cos_ang) + (temp.z * sin_ang);
	vec.y = vec.y;
	vec.z = (-temp.x * sin_ang) + (temp.z * cos_ang);
	return (vec);
}

/*float3	ft_rotate_z(float3 vec, float angle)
{
	float cos_ang = cos(angle);
	float sin_ang = sin(angle);
	
	float3 temp = vec;

	vec.x = (temp.x * cos_ang) + (temp.y * sin_ang);
	vec.y = (temp.x * sin_ang) + (temp.y * cos_ang);
	vec.z = vec.z;
	return (vec);
}*/

float3	ft_vec_rotate(float3 vec, float dx, float dy)
{
	vec = ft_rotate_x(vec, dy);
	vec = ft_rotate_y(vec, dx);
	vec = ft_vec_normalize(vec);
	return (vec);
}

int			ft_color_convert(int color, float lum)
{
	int r = ((color >> 16) & 0xFF) * lum;
	int g = ((color >> 8) & 0xFF) * lum;
	int b = (color & 0xFF) * lum;

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int         ft_sum_color(int c1, int c2, float k1, float k2)
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
float		ft_sph_intersect(float3 origin, float3 dir, float3 obj_pos, float obj_radius)
{
	float3 oc;
	oc.x = origin.x - obj_pos.x;
	oc.y = origin.y - obj_pos.y;
	oc.z = origin.z - obj_pos.z;

	float k1 = ft_dot_prod(dir, dir);
	float k2 = 2.0f * ft_dot_prod(oc, dir);
	float k3 = ft_dot_prod(oc, oc) - (obj_radius * obj_radius);

	float discr = k2 * k2 - (4.0f * k1 * k3);
	if (discr < 0.0f)
		return (-1.0f);
	
	float discr_sqrt = sqrt(discr);
	float k1_ = 2.0f * k1;

	float t1 = (float)(-k2 + discr_sqrt) / (float)k1_;
	float t2 = (float)(-k2 - discr_sqrt) / (float)k1_;
	if (t1 < t2)
		return (t1);
	return (t2);
}

float		ft_plane_intersect(float3 origin, float3 dir, float3 obj_pos, float3 obj_normal)
{
	float denom = ft_dot_prod(obj_normal, dir);
	float denom_abs = denom;
	if (denom < 0.0f)
		denom_abs = -denom;
	if (denom_abs > 0.000001f)
	{
		float3 oc;
		oc.x = obj_pos.x - origin.x;
		oc.y = obj_pos.y - origin.y;
		oc.z = obj_pos.z - origin.z;

		float t = (float)ft_dot_prod(oc, obj_normal) / (float)denom;
		if (t > 0.000001f)
			return (t);
	}
	return (-1.0f);
}

float		ft_cone_intersect(float3 origin, float3 dir, float3 obj_pos, float3 obj_normal, float obj_radius)
{
	float3 oc;
	oc.x = origin.x - obj_pos.x;
	oc.y = origin.y - obj_pos.y;
	oc.z = origin.z - obj_pos.z;

	float pow_ = pow(tan(obj_radius), 2);

	float oc_dot_n = ft_dot_prod(oc, obj_normal);
	float d_dot_n = ft_dot_prod(dir, obj_normal);

	float k1 = ft_dot_prod(dir, dir) - (1.0f + pow_) * pow(d_dot_n, 2);
	float k2 = 2.0f * (ft_dot_prod(dir, oc) - (1.0f + pow_) * d_dot_n * oc_dot_n);
	float k3 = ft_dot_prod(oc, oc) - (1.0f + pow_) * pow(oc_dot_n, 2);

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (-1.0f);

	float discr_sqrt = sqrt(discr);
	float k1_ = 2.0f * k1;
	float t1 = (float)(-k2 + discr_sqrt) / (float)k1_;
	float t2 = (float)(-k2 - discr_sqrt) / (float)k1_;

	if ((t1 < t2 && t1 >= 0.0f) || (t1 >= 0.0f && t2 < 0.0))
		return (t1);
	if ((t2 < t1 && t2 >= 0.0f) || (t2 >= 0.0f && t1 < 0.0f))
		return (t2);
	return (-1.0f);
}

float		ft_cylinder_intersect(float3 origin, float3 dir, float3 obj_pos, float3 obj_normal, float obj_radius)
{
	float3 oc;
	oc.x = origin.x - obj_pos.x;
	oc.y = origin.y - obj_pos.y;
	oc.z = origin.z - obj_pos.z;

	//ft_vec_normalize(&obj_normal.x, &obj_normal.y, &obj_normal.z);

	float d_dot_n = ft_dot_prod(dir, obj_normal);
	float oc_dot_n = ft_dot_prod(oc, obj_normal);

	float k1 = ft_dot_prod(dir, dir) - pow(d_dot_n, 2);
	float k2 = 2.0f * (ft_dot_prod(dir, oc) - (d_dot_n * oc_dot_n));
	float k3 = ft_dot_prod(oc, oc) - pow(oc_dot_n, 2) - pow(obj_radius, 2);

	float discr = k2 * k2 - 4.0f * k1 * k3;
	if (discr < 0.0f)
		return (-1.0f);

	float discr_sqrt = sqrt(discr);
	float k1_ = 2.0f * k1;

	float t1 = (float)(-k2 + discr_sqrt) / (float)k1_;
	float t2 = (float)(-k2 - discr_sqrt) / (float)k1_;

	if (t1 < t2)
		return (t1);
	return (t2);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////	NORMAL		///////////////////////////////////////////////////////////////////////////
float3		ft_sph_normal_calc(float3 dir_, float3 point, float3 obj_pos, float3 normal)
{
	normal.x = point.x - obj_pos.x;
	normal.y = point.y - obj_pos.y;
	normal.z = point.z - obj_pos.z;
	normal = ft_vec_normalize(normal);
	return (normal);
}

float3		ft_plane_normal_calc(float3 dir, float3 obj_normal, float3 normal)
{
	if (ft_dot_prod(dir, obj_normal) > 0.0f)
	{
		normal.x = -obj_normal.x;
		normal.y = -obj_normal.y;
		normal.z = -obj_normal.z;
	}
	else
	{
		normal.x = obj_normal.x;
		normal.y = obj_normal.y;
		normal.z = obj_normal.z;
	}
	return (normal);
}

float3		ft_cone_normal_calc(float3 dir, float3 point, float3 obj_pos, float3 obj_normal, float obj_radius, float3 normal)
{
	normal.x = point.x - obj_pos.x;
	normal.y = point.y - obj_pos.y;
	normal.z = point.z - obj_pos.z;

	float side_len = (float)sqrt(ft_dot_prod(normal, normal)) / (float)cos(obj_radius);

	if (ft_dot_prod(normal, obj_normal) < 0.0f)
		side_len = -side_len;

	normal.x = obj_pos.x + obj_normal.x * side_len;
	normal.y = obj_pos.y + obj_normal.y * side_len;
	normal.z = obj_pos.z + obj_normal.z * side_len;

	normal.x = point.x - normal.x;
	normal.y = point.y - normal.y;
	normal.z = point.z - normal.z;
	normal = ft_vec_normalize(normal);
	return (normal);
}

float3		ft_cylinder_normal_calc(float3 dir, float3 point, float3 obj_pos, float3 obj_normal, float3 normal)
{
	normal.x = point.x - obj_pos.x;
	normal.y = point.y - obj_pos.y;
	normal.z = point.z - obj_pos.z;

	float dis = ft_dot_prod(normal, obj_normal);

	normal.x = obj_pos.x + obj_normal.x * dis;
	normal.y = obj_pos.y + obj_normal.y * dis;
	normal.z = obj_pos.z + obj_normal.z * dis;

	normal.x = point.x - normal.x;
	normal.y = point.y - normal.y;
	normal.z = point.z - normal.z;
	normal = ft_vec_normalize(normal);
	return (normal);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int		ft_shadow_intersection(float3 origin, float3 dir,
								__global float3 *obj_pos,
								__global float3 *obj_normal,
								__global float *obj_radius, int obj_count, __global int *obj_type,
								float min_dist, float max_dist, int obj_i)
{
	float closest = max_dist;
	int i = -1;
	while (++i < obj_count)
	{
		float t;

		if (obj_type[i] == SPHERE)
			t = ft_sph_intersect(origin, dir, obj_pos[i], obj_radius[i]);
		else if (obj_type[i] == PLANE)
			t = ft_plane_intersect(origin, dir, obj_pos[i], obj_normal[i]);
		else if (obj_type[i] == CONE)
			t = ft_cone_intersect(origin, dir, obj_pos[i], obj_normal[i], obj_radius[i]);
		else if (obj_type[i] == CYLINDER)
			t = ft_cylinder_intersect(origin, dir, obj_pos[i], obj_normal[i], obj_radius[i]);
		else
			return (-1);

		if (t >= min_dist && t < closest && obj_i != i)
			return (i);
	}
	return (-1);
}

int		ft_closest_intersection(float3 origin, float3 dir,
								__global float3 *obj_pos,
								__global float3 *obj_normal,
								__global float *obj_radius, int obj_count, __global int *obj_type,
								float min_dist, float max_dist, float *closest, int refl_i)
{
	int obj_i = -1;
	*closest = max_dist;
	int i = -1;
	while (++i < obj_count)
	{
		//float t = ft_sph_intersect(origin_x, origin_y, origin_z, dir_x, dir_y, dir_z, obj_x[i], obj_y[i], obj_z[i], obj_radius[i]);
		float t;

		if (obj_type[i] == SPHERE)
			t = ft_sph_intersect(origin, dir, obj_pos[i], obj_radius[i]);
		else if (obj_type[i] == PLANE)
			t = ft_plane_intersect(origin, dir, obj_pos[i], obj_normal[i]);
		else if (obj_type[i] == CONE)
			t = ft_cone_intersect(origin, dir, obj_pos[i], obj_normal[i], obj_radius[i]);
		else if (obj_type[i] == CYLINDER)
			t = ft_cylinder_intersect(origin, dir, obj_pos[i], obj_normal[i], obj_radius[i]);
		else
			return (-1);

		if (t >= min_dist && t < *closest && refl_i != i)
		{
			obj_i = i;
			*closest = t;
		}
	}
	return (obj_i);
}

int		ft_trace_refl_ray(float3 origin, float3 dir,
							__global float3 *obj_pos,
							__global float3 *obj_normal,
							__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
							__global float *obj_mirrored, int obj_count, __global int *obj_type,
							__global float3 *light_vec,
							__global int *light_type, __global float *light_intensity, int light_count,
							float min_dist, float max_dist, int depth, int refl_i)
{
	int color = ft_trace_ray(origin, dir, obj_pos, obj_normal,
							obj_radius, obj_color, obj_specular,
							obj_mirrored, obj_count, obj_type,
							light_vec,
							light_type, light_intensity, light_count,
							min_dist, max_dist, depth, refl_i);
	return (color);
}

int		ft_trace_ray(float3 origin, float3 dir,
					__global float3 *obj_pos,
					__global float3 *obj_normal,
					__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
					__global float *obj_mirrored, int obj_count, __global int *obj_type,
					__global float3 *light_vec,
					__global int *light_type, __global float *light_intensity, int light_count,
					float min_dist, float max_dist, int depth, int refl_i)
{
	float closest;
	int obj_i = ft_closest_intersection(origin, dir, obj_pos, obj_normal,
										obj_radius, obj_count, obj_type,
										min_dist, max_dist, &closest, refl_i);
	if (obj_i < 0)
		return (BACKGROUND_COLOR);
	//return (obj_color[obj_i]);

	float3 point;
	point.x = origin.x + closest * dir.x;
	point.y = origin.y + closest * dir.y;
	point.z = origin.z + closest * dir.z;

	float3 normal;

	if (obj_type[obj_i] == SPHERE)
		normal = ft_sph_normal_calc(dir, point, obj_pos[obj_i], normal);
	else if (obj_type[obj_i] == PLANE)
		normal = ft_plane_normal_calc(dir, obj_normal[obj_i], normal);
	else if (obj_type[obj_i] == CONE)
		normal = ft_cone_normal_calc(dir, point, obj_pos[obj_i], obj_normal[obj_i], obj_radius[obj_i], normal);
	else if (obj_type[obj_i] == CYLINDER)
		normal = ft_cylinder_normal_calc(dir, point, obj_pos[obj_i], obj_normal[obj_i], normal);

	float3 neg_dir;
	neg_dir.x = -dir.x;
	neg_dir.y = -dir.y;
	neg_dir.z = -dir.z;

	float3 light_dir;
	float s_max;

	float intensity = 0.0f;
	int i = -1;
	while (++i < light_count)
	{
		if (light_type[i] == AMBIENT_L)
			intensity += light_intensity[i];
		else
		{
			if (light_type[i] == POINT_L)
			{
				light_dir.x = light_vec[i].x - point.x;
				light_dir.y = light_vec[i].y - point.y;
				light_dir.z = light_vec[i].z - point.z;
				s_max = 1.0f;
			}
			else
				continue;

			float s_closest;
			int s_obj_i = ft_shadow_intersection(point, light_dir, obj_pos, obj_normal,
												obj_radius, obj_count, obj_type,
												0.000001f, s_max, obj_i);
			if (s_obj_i >= 0)
				continue;

			float n_dot_l = ft_dot_prod(normal, light_dir);
			if (n_dot_l > 0.0f)
				intensity += (float)(light_intensity[i] * n_dot_l) / (float)(ft_vec_len(normal) * ft_vec_len(light_dir));

			if (obj_specular[obj_i] > 0.0f)
			{
				float dot = ft_dot_prod(normal, light_dir);
				float3 s_refl;
				s_refl.x = 2.0f * normal.x * dot - light_dir.x;
				s_refl.y = 2.0f * normal.y * dot - light_dir.y;
				s_refl.z = 2.0f * normal.z * dot - light_dir.z;

				float r_dot_v = ft_dot_prod(s_refl, neg_dir);
				if (r_dot_v > 0.0f)
					intensity += (light_intensity[i] * pow((float)r_dot_v / (float)(ft_vec_len(s_refl) * ft_vec_len(neg_dir)), (int)obj_specular[obj_i]));
			}
		}
	}

	if (intensity > 1.0f)
		intensity = 1.0f;

	int color = ft_color_convert(obj_color[obj_i], intensity);
	if (depth > 1 || obj_mirrored[obj_i] <= 0)
		return (color);

	float refl_dot = ft_dot_prod(neg_dir, normal);

	float3 refl_ray;
	refl_ray.x = 2.0f * refl_dot * normal.x - neg_dir.x;
	refl_ray.y = 2.0f * refl_dot * normal.y - neg_dir.y;
	refl_ray.z = 2.0f * refl_dot * normal.z - neg_dir.z;

	int refl_color = ft_trace_refl_ray(point, refl_ray, obj_pos, obj_normal,
										obj_radius, obj_color, obj_specular,
										obj_mirrored, obj_count, obj_type,
										light_vec,
										light_type, light_intensity, light_count,
										0.000001f, MAX_FLT, depth + 1, obj_i);

	return (ft_sum_color(color, refl_color, 1 - obj_mirrored[obj_i], obj_mirrored[obj_i]));
}

__kernel void render(__global unsigned int *buffer,
							float origin_x, float origin_y, float origin_z,
							__global float3 *obj_pos,
							__global float3 *obj_normal,
							__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
							__global float *obj_mirrored, int obj_count, __global int *obj_type,
							__global float3 *light_vec,
							__global float *light_type, __global float *light_intensity, int light_count,
							float dx, float dy)
{
	unsigned int pixel = get_global_id(0);

	int y = pixel / W - (H / 2.0f);
	int x = pixel % W - (W / 2.0f);

	float3 origin;
	origin.x = origin_x;
	origin.y = origin_y;
	origin.z = origin_z;

	float3 dir;
	dir.x = (float)x / (float)W * (float)AR;
	dir.y = -(float)y / (float)H;
	dir.z = 1.0f;

	dir = ft_vec_rotate(dir, dx, dy);

	int color = ft_trace_ray(origin, dir, obj_pos, obj_normal,
							obj_radius, obj_color, obj_specular,
							obj_mirrored, obj_count, obj_type,
							light_vec, light_type, light_intensity, light_count,
							1.0f, MAX_FLT, 0, -1);

	buffer[pixel] = color;
}
