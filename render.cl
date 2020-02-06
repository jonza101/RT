#define W 1600
#define H 900
#define AR ((float)W / (float)H)
#define WH (W * H)

#define BACKGROUND_COLOR 0x0

#define MAX_DEPTH 1

#define SPHERE 0
#define PLANE 1
#define CONE 2
#define CYLINDER 3

#define AMBIENT_L 0
#define POINT_L 1

#define NONE 0
#define CEL_SHADING 1
#define SEPIA 2
#define GRAYSCALE 3
#define BLACK_WHITE 4

#define MAX_FLT 3.40282346638528859811704183484516925e+38F


typedef	struct		s_rand
{
	float			seed;
	float			fgi;
}					t_rand;

typedef struct		s_effect
{
	int				effect_type;
	int				cel_band;
	int				negative;
	int 			soft_shadows;
	int				ss_cell;
	int				bw_factor;
}                  	t_effect;

int		ft_trace_ray(float3 origin, float3 dir,
					__global float3 *obj_pos,
					__global float3 *obj_normal,
					__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
					__global float *obj_mirrored, __global float *obj_transparency, __global float *obj_refractive_index,
					int obj_count, __global int *obj_type,
					__global float3 *light_vec,
					__global int *light_type, __global float *light_intensity, int light_count,
					float min_dist, float max_dist, int depth, int refl_i, t_effect effect, t_rand srnd,
					__global ulong4 *obj_txt, __global int3 *obj_txt_misc);


//random values in range 0.0 to 1.0
static float rand(float x, float y, float z, t_rand srnd)
{
    float ptr = 0.0f;
	float rnd = fract(sin(x*112.9898f + y*179.233f + z*237.212f) * 43758.5453f, &ptr);
	srnd.seed = rnd;
    return (rnd);
}

float		ft_min(float a, float b)
{
	if (b < a)
		return (b);
	return (a);
}

float		ft_max(float a, float b)
{
	if (b > a)
		return (b);
	return (a);
}

float	ft_clamp(float a, float min_val, float max_val)
{
	return (min(max(a, min_val), max_val));
}

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

float3		ft_cross_prod(float3 a, float3 b)
{
	float3 vec;
	vec.x = a.y * b.z - a.z * b.y;
	vec.y = a.z * b.x - a.x * b.z;
	vec.z = a.x * b.y - a.y * b.x;
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

	r = (r > 255) ? 255 : r;
	g = (g > 255) ? 255 : g;
	b = (b > 255) ? 255 : b;

    return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int			ft_to_sepia(int color)
{
	int r = (color >> 16) & 0xFF;
	int g = (color >> 8) & 0xFF;
	int b = color & 0xFF;

	int rr = (r * 0.393f) + (g * 0.769f) + (b * 0.189f);
	int gg = (r * 0.349f) + (g * 0.686f) + (b * 0.168f);
	int bb = (r * 0.272f) + (g * 0.534f) + (b * 0.131f);

	rr = (rr > 255) ? 255 : rr;
	gg = (gg > 255) ? 255 : gg;
	bb = (bb > 255) ? 255 : bb;

	return (((rr & 0xFF) << 16) + ((gg & 0xFF) << 8) + ((bb & 0xFF)));
}

int			ft_to_grayscale(int color)
{
	int r = (color >> 16) & 0xFF;
	int g = (color >> 8) & 0xFF;
	int b = color & 0xFF;

	int rr = (r * 0.299f) + (g * 0.587f) + (b * 0.114f);
	int gg = (r * 0.299f) + (g * 0.587f) + (b * 0.114f);
	int bb = (r * 0.299f) + (g * 0.587f) + (b * 0.114f);

	rr = (rr > 255) ? 255 : rr;
	gg = (gg > 255) ? 255 : gg;
	bb = (bb > 255) ? 255 : bb;

	return (((rr & 0xFF) << 16) + ((gg & 0xFF) << 8) + ((bb & 0xFF)));
}	

int			ft_to_negative(int color)
{
	int r = 255 - (color >> 16) & 0xFF;
	int g = 255 - (color >> 8) & 0xFF;
	int b = 255 - color & 0xFF;

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int			ft_to_black_white(int color, int factor)
{
	int r = 255 - (color >> 16) & 0xFF;
	int g = 255 - (color >> 8) & 0xFF;
	int b = 255 - color & 0xFF;

	int sum = r + b + g;
	if (sum > ((255.0f + factor)  / 2.0f) * 3.0f)
	{
		r = 255;
		g = 255;
		b = 255;
	}
	else
	{
		r = 0;
		g = 0;
		b = 0;
	}

	return (((r & 0xFF) << 16) + ((g & 0xFF) << 8) + ((b & 0xFF)));
}

int			ft_to_noise(int color, int factor, t_rand srnd)
{
	int rnd = rand(srnd.fgi, 0.0f, srnd.seed, srnd) * factor - factor;

	int r = ((color >> 16) & 0xFF) + rnd;
	int g = ((color >> 8) & 0xFF) + rnd;
	int b = (color & 0xFF) + rnd;

	r = (r < 0) ? 0 : r;
	g = (g < 0) ? 0 : g;
	b = (b < 0) ? 0 : b;
	r = (r > 255) ? 255 : r;
	g = (g > 255) ? 255 : g;
	b = (b > 255) ? 255 : b;

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3		ft_vec_transform(float3 obj_pos, float3 obj_normal, float3 normal, float3 point)
{
	float3 norm = obj_normal;
	float3 vec = point - obj_pos;

	float cos_x, cos_z;
	float3 n_vec, n_norm;
	float alpha_x, alpha_z;

	float norm_len = ft_vec_len(norm);
	if (norm_len < 1e-5f)
		return (vec);
	else
	{
		cos_x = (float)norm.x / (float)norm_len;
		alpha_x = (norm.y > 0.0f) ? acos(cos_x) : -acos(cos_x);
	}

	float cos_alpha_x = cos(alpha_x);
	float sin_alpha_x = sin(alpha_x);
	
	float3 temp;
	temp = (float3)(cos_alpha_x, sin_alpha_x, 0.0f);
	n_vec.x = ft_dot_prod(temp, vec);
	n_norm.x = ft_dot_prod(temp, norm);

	temp = (float3)(-sin_alpha_x, cos_alpha_x, 0.0f);
	n_vec.y = ft_dot_prod(temp, vec);
	n_norm.y = ft_dot_prod(temp, norm);

	temp = (float3)(0.0f, 0.0f, 1.0f);
	n_vec.z = ft_dot_prod(temp, vec);
	n_norm.z = ft_dot_prod(temp, norm);

	/*n_vec.x = ft_dot_prod((float3)(cos_alpha_x, sin_alpha_x, 0.0f), vec);
	n_vec.y = ft_dot_prod((float3)(-sin_alpha_x, cos_alpha_x, 0.0f), vec);
	n_vec.z = ft_dot_prod((float3)(0.0f, 0.0f, 1.0f), vec);

	n_norm.x = ft_dot_prod((float3)(cos_alpha_x, sin_alpha_x, 0.0f), norm);
	n_norm.y = ft_dot_prod((float3)(-sin_alpha_x, cos_alpha_x, 0.0f), norm);
	n_norm.z = ft_dot_prod((float3)(0.0f, 0.0f, 1.0f), norm);*/

	cos_z = (float)n_norm.z / (float)ft_vec_len(n_norm);
	alpha_z = (n_norm.x > 0.0f) ? acos(cos_z) : -acos(cos_z);

	float cos_alpha_z = cos(alpha_z);
	float sin_alpha_z = sin(alpha_z);

	vec.x = ft_dot_prod((float3)(cos_alpha_z, 0.0f, -sin_alpha_z), n_vec);
	vec.y = ft_dot_prod((float3)(0.0f, 1.0f, 0.0f), n_vec);
	vec.z = ft_dot_prod((float3)(sin_alpha_z, 0.0f, cos_alpha_z), n_vec);

	return (vec);
}

int		ft_sph_txt_map(__global ulong4 *obj_txt, int txt_idx, float3 normal)
{
	float u = ft_clamp(0.5f + (float)atan2(normal.z, normal.x) / (float)(2.0f * 3.14159f), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - (float)asin(normal.y) / (float)3.14159f, 0.0f, 1.0f);
	int tx = (float)u * (float)obj_txt[txt_idx].x;
	int ty = (float)v * (float)obj_txt[txt_idx].y;
	int color = obj_txt[ty * obj_txt[txt_idx].x + tx + obj_txt[txt_idx].w].z;
	return (color);
}

int		ft_plane_txt_map(__global ulong4 *obj_txt, int txt_idx, float3 normal, float3 point)
{
	float3 vec_temp;

	if (normal.x != 0.0f || normal.y != 0.0f)
	{
		vec_temp.x = normal.y;
		vec_temp.y = -normal.x;
		vec_temp.z = 0.0f;
		vec_temp = ft_vec_normalize(vec_temp);
	}
	else
	{
		vec_temp.x = 0.0f;
		vec_temp.y = 1.0f;
		vec_temp.z = 0.0f;
		vec_temp = ft_vec_normalize(vec_temp);
	}

	float3 vec_tmp = ft_cross_prod(normal, vec_temp);
	float u = ft_clamp(0.5f + (float)fmod(ft_dot_prod(vec_temp, point), 4.0f) / 8.0f, 0.0f, 1.0f);
	float v = ft_clamp(0.5f + (float)fmod(ft_dot_prod(vec_tmp, point), 4.0f) / 8.0f, 0.0f, 1.0f);
	int tx = (float)u * (float)obj_txt[txt_idx].x;
	int ty = (float)v * (float)obj_txt[txt_idx].y;
	int color = obj_txt[ty * obj_txt[txt_idx].x + tx + obj_txt[txt_idx].w].z;
	return (color);
}

int		ft_cylinder_txt_map(__global ulong4 *obj_txt, int txt_idx, float3 obj_pos, float3 obj_normal, float obj_radius, float3 normal, float3 point)
{
	float3 vec = ft_vec_transform(obj_pos, obj_normal, normal, point);
	float u = ft_clamp(0.5f + ((float)atan2(vec.x, vec.y) / (float)(2.0f * 3.14159f)), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - (modf((float)vec.z / (float)obj_radius * 0.25f, &v) * 0.5f), 0.0f, 1.0f);
	int tx = (float)u * (float)obj_txt[txt_idx].x;
	int ty = (float)v * (float)obj_txt[txt_idx].y;
	int color = obj_txt[ty * obj_txt[txt_idx].x + tx + obj_txt[txt_idx].w].z;
	return (color);
}

int		ft_cone_txt_map(__global ulong4 *obj_txt, int txt_idx, float3 obj_pos, float3 obj_normal, float obj_radius, float3 normal, float3 point)
{
	float3 vec = ft_vec_transform(obj_pos, obj_normal, normal, point);
	float p = (float)((float)vec.x / (float)vec.z) / (float)tan(obj_radius);
	float u = ft_clamp((float)((vec.y > 0.0) ? acos(p) : (2.0f * 3.14159f - acos(p))) / (float)(2.0f * 3.14159f), 0.0f, 1.0f);
	float v = ft_clamp(0.5f - modf(vec.z * 0.5f, &v) * 0.5f, 0.0f, 1.0f);
	int tx = (float)u * (float)obj_txt[txt_idx].x;
	int ty = (float)v * (float)obj_txt[txt_idx].y;
	int color = obj_txt[ty * obj_txt[txt_idx].x + tx + obj_txt[txt_idx].w].z;
	return (color);
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

float3	ft_refract(float3 dir, float3 normal, float refractive_index, float3 refr_ray)
{
	float cosi = ft_clamp(-1.0f, 1.0f, ft_dot_prod(dir, normal));
	float etai = 1.0f;
	float etat = refractive_index;

	float3 n = normal;
	if (cosi < 0.0f)
		cosi = -cosi;
	else
	{
		float temp = etai;
		etai = etat;
		etat = temp;

		n.x = -normal.x;
		n.y = -normal.y;
		n.z = -normal.z;
	}

	float eta = (float)etai / (float)etat;
	float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
	if (k < 0.0f)
	{
		refr_ray.x	=  0.0f;
		refr_ray.y	=  0.0f;
		refr_ray.z	=  0.0f;
	}
	else
	{
		refr_ray.x = eta * dir.x + (eta * cosi - sqrt(k)) * n.x;
		refr_ray.y = eta * dir.y + (eta * cosi - sqrt(k)) * n.y;
		refr_ray.z = eta * dir.z + (eta * cosi - sqrt(k)) * n.z;
	}


	/*float n = refractive_index;
	float cosi = -(ft_dot_prod(normal, dir));

	if (cosi < 0.0f)
		cosi = -cosi;
	else
	{
		n = 1.0f / (float)n;
		normal = -normal;
	}
	float sint2 = n * n * (1.0f - cosi * cosi);
	float cost = sqrt(1.0f - sint2);

	refr_ray.x = n * dir.x + (n * cosi - cost) * normal.x;
	refr_ray.y = n * dir.y + (n * cosi - cost) * normal.y;
	refr_ray.z = n * dir.z + (n * cosi - cost) * normal.z;*/

	return (refr_ray);
}

int		ft_trace_ray_rec(float3 origin, float3 dir,
							__global float3 *obj_pos,
							__global float3 *obj_normal,
							__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
							__global float *obj_mirrored, __global float *obj_transparency, __global float *obj_refractive_index,
							int obj_count, __global int *obj_type,
							__global float3 *light_vec,
							__global int *light_type, __global float *light_intensity, int light_count,
							float min_dist, float max_dist, int depth, int refl_i, t_effect effect, t_rand srnd,
							__global ulong4 *obj_txt, __global int3 *obj_txt_misc)
{
	int color = ft_trace_ray(origin, dir, obj_pos, obj_normal,
							obj_radius, obj_color, obj_specular,
							obj_mirrored, obj_transparency, obj_refractive_index,
							obj_count, obj_type,
							light_vec,
							light_type, light_intensity, light_count,
							min_dist, max_dist, depth, refl_i, effect, srnd,
							obj_txt, obj_txt_misc);
	return (color);
}

int		ft_trace_ray(float3 origin, float3 dir,
					__global float3 *obj_pos,
					__global float3 *obj_normal,
					__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
					__global float *obj_mirrored, __global float *obj_transparency, __global float *obj_refractive_index,
					int obj_count, __global int *obj_type,
					__global float3 *light_vec,
					__global int *light_type, __global float *light_intensity, int light_count,
					float min_dist, float max_dist, int depth, int refl_i, t_effect effect, t_rand srnd,
					__global ulong4 *obj_txt, __global int3 *obj_txt_misc)
{
	float closest;
	int obj_i = ft_closest_intersection(origin, dir, obj_pos, obj_normal,
										obj_radius, obj_count, obj_type,
										min_dist, max_dist, &closest, refl_i);
	if (obj_i < 0)
		return (BACKGROUND_COLOR);

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

			
			float s_i = 0.0f;

			if (!effect.soft_shadows)
			{
				float s_closest;
				int s_obj_i = ft_shadow_intersection(point, light_dir, obj_pos, obj_normal,
													obj_radius, obj_count, obj_type,
													0.000001f, s_max, obj_i);
				if (s_obj_i >= 0)
					s_i = 1.0f - obj_transparency[s_obj_i];
					//continue;
			}
			else
			{
				float x1 = point.x;
				float y1 = point.z;
				float x2 = light_vec[i].x;
				float y2 = light_vec[i].z;
				float len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				float xxx = (float)(1.0f * (y1 - y2)) / (float)len;
				float yyy = (float)(1.0f * (x2 - x1)) / (float)len;

				float2 vec[4];
				vec[0].x = x2 + xxx;
				vec[0].y = y2 + yyy;
				vec[1].x = x2 - xxx;
				vec[1].y = y2 + yyy;
				vec[2].x = x2 + xxx;
				vec[2].y = y2 - yyy;
				vec[3].x = x2 - xxx;
				vec[3].y = y2 - yyy;

				int idx = 0;
				int f = -1;
				while (++f < 4)
				{
					float dx = vec[f].x - x2;
					float dy = vec[f].y - y2;
					float dist = sqrt(dx * dx + dy * dy);
					if (dist == 1.0f)
					{
						idx = f;
						break;
					}
				}

				float3 l_vec = (float3)(vec[idx].x, light_vec[i].y + 1.0f, vec[idx].y);
				float3 diff = (float3)(light_vec[i].x - l_vec.x, l_vec.y, light_vec[i].z - l_vec.z);
				float3 r_vec = (float3)(light_vec[i].x + diff.x, l_vec.y, light_vec[i].z + diff.z);

				float3 v_cell[16];
				float step = (float)(2.0f * 1.0f) / (float)effect.ss_cell;

				float2 temp = (float2)(r_vec.x - l_vec.x, r_vec.z - l_vec.z);
				float temp_len = sqrt(temp.x * temp.x + temp.y * temp.y);
				float2 u = (float2)((float)temp.x / (float)temp_len, (float)temp.y / (float)temp_len);

				int c = -1;
				while (++c < effect.ss_cell)
				{
					v_cell[c].x = l_vec.x + (step * c) * u.x;
					v_cell[c].y = l_vec.y;
					v_cell[c].z = l_vec.z + (step * c) * u.y;
				}

				float sh = 0;

				int xx = -1;
				while (++xx < effect.ss_cell)
				{
					int yy = -1;
					while (++yy < effect.ss_cell)
					{
						//float rnd = rand_val * step;
						float rnd = rand(srnd.fgi, 0.0f, srnd.seed, srnd) * step;

						float cell_x = v_cell[xx].x + rnd;
						float cell_y = v_cell[xx].y - (yy * step) + rnd;
						float cell_z = v_cell[xx].z + rnd;

						float dx = point.x - cell_x;
						float dy = point.y - cell_y;
						float dz = point.z - cell_z;
						float dist = sqrt(dx * dx + dy * dy + dz * dz);

						float3 ss_dir;
						ss_dir.x = (float)(cell_x - point.x) / (float)dist;
						ss_dir.y = (float)(cell_y - point.y) / (float)dist;
						ss_dir.z = (float)(cell_z - point.z) / (float)dist;

						int s_obj_i = ft_shadow_intersection(point, ss_dir, obj_pos, obj_normal,
															obj_radius, obj_count, obj_type,
															0.000001f, dist, obj_i);
						if (s_obj_i >= 0)
						{
							sh += 1.0f;
							sh -= obj_transparency[s_obj_i];
						}
					}
				}
				s_i = (float)sh / (float)(effect.ss_cell * effect.ss_cell);
			}



			float n_dot_l = ft_dot_prod(normal, light_dir);
			if (n_dot_l > 0.0f)
			{
				float intens = (float)(light_intensity[i] * n_dot_l) / (float)(ft_vec_len(normal) * ft_vec_len(light_dir));
				intens = intens * (1.0f - s_i);
				intensity += intens;
			}

			if (obj_specular[obj_i] > 0.0f)
			{
				float dot = ft_dot_prod(normal, light_dir);
				float3 s_refl;
				s_refl.x = 2.0f * normal.x * dot - light_dir.x;
				s_refl.y = 2.0f * normal.y * dot - light_dir.y;
				s_refl.z = 2.0f * normal.z * dot - light_dir.z;

				float r_dot_v = ft_dot_prod(s_refl, neg_dir);
				if (r_dot_v > 0.0f)
				{
					float intens = (light_intensity[i] * pow((float)r_dot_v / (float)(ft_vec_len(s_refl) * ft_vec_len(neg_dir)), (int)obj_specular[obj_i]));
					intens = intens * (1.0f - s_i);
					intensity += intens;
				}
			}
		}
	}

	if (intensity > 1.0f)
		intensity = 1.0f;

	if (effect.effect_type == CEL_SHADING)
	{
		float cel_dot = ft_dot_prod(dir, normal);
		float cel_acc = 1.0f / (float)effect.cel_band;
		float curr_intensity = 0.0f;
		int cel_f = 0;
		int cel_i = -1;
		while (++cel_i < effect.cel_band)
		{
			if (intensity >= curr_intensity && intensity < (curr_intensity + cel_acc))
			{
				intensity = curr_intensity + ((float)cel_acc * 0.5f);
				cel_f = 1;
				break;
			}
			curr_intensity += cel_acc;
		}
	}

	int txt_trans = 0;
	int color = obj_color[obj_i];
	if (obj_txt_misc[obj_i].x >= 0)
	{
		if (obj_type[obj_i] == SPHERE)
			color = ft_sph_txt_map(obj_txt, obj_txt_misc[obj_i].x, normal);
		else if (obj_type[obj_i] == PLANE)
			color = ft_plane_txt_map(obj_txt, obj_txt_misc[obj_i].x, normal, point);
		else if (obj_type[obj_i] == CYLINDER)
			color = ft_cylinder_txt_map(obj_txt, obj_txt_misc[obj_i].x, obj_pos[obj_i], obj_normal[obj_i], obj_radius[obj_i], normal, point);
		else if (obj_type[obj_i] == CONE)
			color = ft_cone_txt_map(obj_txt, obj_txt_misc[obj_i].x, obj_pos[obj_i], obj_normal[obj_i], obj_radius[obj_i], normal, point);
		else
			color = obj_color[obj_i];

		if (obj_txt_misc[obj_i].y && color == obj_txt_misc[obj_i].z)
		{
			color = obj_color[obj_i];
			txt_trans = 1;
		}
	}

	color = ft_color_convert(color, intensity);

	if (obj_mirrored[obj_i] > 0.0f && depth <= MAX_DEPTH && ((obj_txt_misc[obj_i].x >= 0 && txt_trans) || (obj_txt_misc[obj_i].x < 0)))
	{
		float refl_dot = ft_dot_prod(neg_dir, normal);

		float3 refl_ray;
		refl_ray.x = 2.0f * refl_dot * normal.x - neg_dir.x;
		refl_ray.y = 2.0f * refl_dot * normal.y - neg_dir.y;
		refl_ray.z = 2.0f * refl_dot * normal.z - neg_dir.z;

		int refl_color = ft_trace_ray_rec(point, refl_ray, obj_pos, obj_normal,
											obj_radius, obj_color, obj_specular,
											obj_mirrored, obj_transparency, obj_refractive_index,
											obj_count, obj_type,
											light_vec,
											light_type, light_intensity, light_count,
											0.000001f, MAX_FLT, depth + 1, obj_i, effect, srnd,
											obj_txt, obj_txt_misc);

		color = ft_sum_color(color, refl_color, 1.0f - obj_mirrored[obj_i], obj_mirrored[obj_i]);
	}

	if (obj_transparency[obj_i] > 0.0f && depth <= MAX_DEPTH && ((obj_txt_misc[obj_i].x >= 0 && txt_trans) || (obj_txt_misc[obj_i].x < 0)))
	{
		float3 refr_ray = ft_refract(dir, normal, obj_refractive_index[obj_i], refr_ray);

		int trans_color = ft_trace_ray_rec(point, refr_ray, obj_pos, obj_normal,
											obj_radius, obj_color, obj_specular,
											obj_mirrored, obj_transparency, obj_refractive_index,
											obj_count, obj_type,
											light_vec,
											light_type, light_intensity, light_count,
											0.000001f, MAX_FLT, depth + 1, obj_i, effect, srnd,
											obj_txt, obj_txt_misc);

		color = ft_sum_color(color, trans_color, 1.0f - obj_transparency[obj_i], obj_transparency[obj_i]);
	}
	return (color);
}

__kernel void render(__global unsigned int *buffer,
							float origin_x, float origin_y, float origin_z,
							__global float3 *obj_pos,
							__global float3 *obj_normal,
							__global float *obj_radius, __global int *obj_color, __global float *obj_specular,
							__global float *obj_mirrored, __global float *obj_transparency, __global float *obj_refractive_index,
							int obj_count, __global int *obj_type,
							__global float3 *light_vec,
							__global float *light_type, __global float *light_intensity, int light_count,
							float dx, float dy, int effect_type, int cel_band, int negative, int soft_shadows, int ss_cell, float seed,
							int bw_factor, int noise, int ns_factor, __global ulong4 *obj_txt, __global int3 *obj_txt_misc)
{
	unsigned int pixel = get_global_id(0);

	int y = pixel / W - (H * 0.5f);
	int x = pixel % W - (W * 0.5f);

	float3 origin;
	origin.x = origin_x;
	origin.y = origin_y;
	origin.z = origin_z;

	float3 dir;
	dir.x = (float)x / (float)W * (float)AR;
	dir.y = -(float)y / (float)H;
	dir.z = 1.0f;

	dir = ft_vec_rotate(dir, dx, dy);

	t_effect effect;
	effect.effect_type = effect_type;
	effect.cel_band = cel_band;
	effect.negative = negative;
	effect.soft_shadows = soft_shadows;
	effect.ss_cell = ss_cell;
	effect.bw_factor = bw_factor;

	t_rand srnd;
	srnd.seed = seed;
	srnd.fgi = (float)pixel / (float)WH;
	//float rand_val = rand(fgi, 0.0f, seed);

	int color = ft_trace_ray(origin, dir, obj_pos, obj_normal,
							obj_radius, obj_color, obj_specular,
							obj_mirrored, obj_transparency, obj_refractive_index,
							obj_count, obj_type,
							light_vec, light_type, light_intensity, light_count,
							1.0f, MAX_FLT, 0, -1, effect, srnd,
							obj_txt, obj_txt_misc);
	
	if (effect_type == SEPIA)
		color = ft_to_sepia(color);
	else if (effect_type == GRAYSCALE)
		color = ft_to_grayscale(color);
	else if (effect_type == BLACK_WHITE)
		color = ft_to_black_white(color, effect.bw_factor);
	if (negative)
		color = ft_to_negative(color);
	if (noise)
		color = ft_to_noise(color, ns_factor, srnd);

	//int color = ft_color_convert(0xFFFFFF, rand_val);

	buffer[pixel] = color;
}
