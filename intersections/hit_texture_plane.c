#include "../include/minirt.h"

static bool	plane_t(const t_texture_plane *pl, const t_ray *ray, double *t)
{
	double	denom;
	t_vec3	diff;

	denom = vec_dot(ray->direction, pl->normal);
	if (fabs(denom) < EPSILON)
		return (false);
	diff = vec_sub(pl->point, ray->origin);
	*t = vec_dot(diff, pl->normal) / denom;
	return (*t > EPSILON);
}

static void	set_plane_hit(const t_texture_plane *pl, const t_ray *ray, double t,
			t_hit_record *rec)
{
	rec->t = t;
	rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
	rec->normal = pl->normal;
	if (vec_dot(ray->direction, rec->normal) > 0)
	{
		rec->normal = vec_mult(rec->normal, -1);
		rec->front_face = false;
	}
	else
		rec->front_face = true;
}
static void plane_basis(const t_vec3 *normal, t_vec3 *u, t_vec3 *v)
{
	t_vec3 arbitrary = (fabs(normal->y) > 0.9) ? (t_vec3){1,0,0} : (t_vec3){0,1,0};
	*u = vec_normalize(vec_cross(*normal, arbitrary));
	*v = vec_cross(*normal, *u);
}

static void sample_pixel(const t_texture_plane *pl, double u, double v,
		unsigned char *out_rgb, const unsigned char *pixels, int flip_v)
{
	long x = (long)(clamp(u, 0.0, 1.0) * (pl->width - 1));
	long y = (long)(clamp(v, 0.0, 1.0) * (pl->height - 1));
	if (flip_v)
		y = (pl->height - 1) - y;
	const unsigned char *p = pixels + (y * pl->width + x) * 3;
	out_rgb[0] = p[0]; out_rgb[1] = p[1]; out_rgb[2] = p[2];
}

static double sample_height(const t_texture_plane *pl, double u, double v, int flip_v)
{
	unsigned char rgb[3];
	sample_pixel(pl, u, v, rgb, pl->bump_pixels, flip_v);
	return (rgb[0] + rgb[1] + rgb[2]) / (3.0 * 255.0);
}

static t_vec3 perturb_normal_from_height(const t_texture_plane *pl,
		const t_vec3 *normal, const t_vec3 *u_axis, const t_vec3 *v_axis,
		double u_uv, double v_uv, double strength)
{
	double du = 1.0 / (double)pl->width;
	double dv = 1.0 / (double)pl->height;

	/* keep sampling orientation consistent with color sampling (flip v) */
	int flip_v = 1;
	double hL = sample_height(pl, u_uv - du, v_uv, flip_v);
	double hR = sample_height(pl, u_uv + du, v_uv, flip_v);
	double hD = sample_height(pl, u_uv, v_uv - dv, flip_v);
	double hU = sample_height(pl, u_uv, v_uv + dv, flip_v);

	double dHdu = (hR - hL) * 0.5;
	double dHdv = (hU - hD) * 0.5;

	t_vec3 pert = *normal;
	pert = vec_add(pert, vec_mult(*u_axis, -dHdu * strength));
	pert = vec_add(pert, vec_mult(*v_axis, -dHdv * strength));
	return vec_normalize(pert);
}

bool	hit_texture_plane(const t_texture_plane *pl, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	double	t;

	if (!plane_t(pl, ray, &t) || t >= t_max)
		return (false);
	set_plane_hit(pl, ray, t, rec);

	t_vec3 L = vec_sub(rec->point, pl->point);

	t_vec3 u_axis, v_axis;
	plane_basis(&pl->normal, &u_axis, &v_axis);

	double u_coord = vec_dot(L, u_axis);
	double v_coord = vec_dot(L, v_axis);

	if (fabs(u_coord) > pl->width_m * 0.5 || fabs(v_coord) > pl->height_m * 0.5)
		return (false);
	/* map to texture pixel coordinates */
	double u_uv = (u_coord / pl->width_m) + 0.5; /* 0..1 */
	double v_uv = (v_coord / pl->height_m) + 0.5; /* 0..1 */
	/* apply per-object UV scale if available */
	if (rec && rec->obj)
	{
		u_uv *= rec->obj->uv_scale_u;
		v_uv *= rec->obj->uv_scale_v;
		u_uv = u_uv - floor(u_uv);
		v_uv = v_uv - floor(v_uv);
	}

	unsigned char rgb[3];
	/* sample color and bump using same vertical orientation (flip v = 1) */
	sample_pixel(pl, u_uv, v_uv, rgb, pl->pixels, 1);
	rec->color = (t_color){ (double)rgb[0], (double)rgb[1], (double)rgb[2] };
	if (pl->bump_pixels)
	{
		rec->normal = perturb_normal_from_height(pl, &rec->normal, &u_axis, &v_axis, u_uv, v_uv, 5.0);
		/* recompute front face after normal perturbation */
		if (vec_dot(ray->direction, rec->normal) > 0)
		{
			rec->normal = vec_mult(rec->normal, -1);
			rec->front_face = false;
		}
		else
			rec->front_face = true;
	}
	return (true);
}
