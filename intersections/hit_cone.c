#include "../include/minirt.h"

static bool	solve_quadratic(double a, double b, double c, double *t)
{
	double	discriminant;
	double	sqrt_disc;
	double	t0;
	double	t1;

	if (fabs(a) < EPSILON)
		return (false);
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0.0)
		return (false);
	sqrt_disc = sqrt(discriminant);
	t0 = (-b - sqrt_disc) / (2.0 * a);
	t1 = (-b + sqrt_disc) / (2.0 * a);
	if (t0 > EPSILON && (t0 < t1 || t1 <= EPSILON))
	{
		*t = t0;
		return (true);
	}
	if (t1 > EPSILON)
	{
		*t = t1;
		return (true);
	}
	return (false);
}
static t_vec3	cone_coeffs(const t_cone *cy, const t_ray *ray)
{
	t_vec3	oc;
	t_vec3	dir;
	double	k;
	t_vec3	coeffs;

	oc = vec_sub(ray->origin, cy->apex);
	dir = cy->center_dir;
	k = (cy->diameter * 0.5) / cy->height;
	k = k * k;
	coeffs.x = vec_dot(ray->direction, ray->direction)
		- (1.0 + k) * vec_dot(ray->direction, dir)
		* vec_dot(ray->direction, dir);
	coeffs.y = 2.0 * (vec_dot(ray->direction, oc)
			- (1.0 + k) * vec_dot(ray->direction, dir)
			* vec_dot(oc, dir));
	coeffs.z = vec_dot(oc, oc)
		- (1.0 + k) * vec_dot(oc, dir) * vec_dot(oc, dir);
	return (coeffs);
}

static void	set_cone_hit(const t_cone *cy, const t_ray *ray, double t,
			t_hit_record *rec)
{
	t_vec3	hit_point;
	t_vec3	dir;

	rec->t = t;
	hit_point = vec_add(ray->origin, vec_mult(ray->direction, t));
	rec->point = hit_point;
	dir = cy->center_dir;
	rec->normal = vec_sub(vec_normalize(vec_sub(hit_point, cy->apex)),
			vec_mult(dir,
				vec_dot(vec_sub(hit_point, cy->apex), dir)
				* (1.0 + (cy->diameter * 0.5 / cy->height)
					* (cy->diameter * 0.5 / cy->height))));
	if (vec_dot(ray->direction, rec->normal) > 0)
	{
		rec->normal = vec_mult(rec->normal, -1);
		rec->front_face = false;
	}
	else
		rec->front_face = true;
}

bool	hit_cone(const t_cone *cy, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	t_vec3	coeffs;
	double	t;

	coeffs = cone_coeffs(cy, ray);
	if (!solve_quadratic(coeffs.x, coeffs.y, coeffs.z, &t) || t >= t_max)
		return (false);
	set_cone_hit(cy, ray, t, rec);
	return (true);
}
