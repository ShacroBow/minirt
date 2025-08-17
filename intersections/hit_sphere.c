#include "../minirt.h"

static bool	solve_quadratic(double a, double b, double c, double *t)
{
	double	discriminant;
	double	root;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (false);
	root = (-b - sqrt(discriminant)) / (2.0 * a);
	if (root > EPSILON)
	{
		*t = root;
		return (true);
	}
	root = (-b + sqrt(discriminant)) / (2.0 * a);
	if (root > EPSILON)
	{
		*t = root;
		return (true);
	}
	return (false);
}

static void	sphere_coeffs(const t_sphere *sp, const t_ray *ray,
			double *a, double *b, double *c)
{
	double	r;
	t_vec3	oc;

	oc = vec_sub(ray->origin, sp->center);
	*a = vec_length_squared(ray->direction);
	*b = 2.0 * vec_dot(oc, ray->direction);
	r = sp->diameter * 0.5;
	*c = vec_length_squared(oc) - (r * r);
}

static void	set_sphere_hit(const t_sphere *sp, const t_ray *ray, double t,
			t_hit_record *rec)
{
	rec->t = t;
	rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
	rec->normal = vec_normalize(vec_sub(rec->point, sp->center));
	if (vec_dot(ray->direction, rec->normal) > 0)
	{
		rec->normal = vec_mult(rec->normal, -1);
		rec->front_face = false;
	}
	else
		rec->front_face = true;
}

bool	hit_sphere(const t_sphere *sp, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	double	a;
	double	b;
	double	c;
	double	t;

	sphere_coeffs(sp, ray, &a, &b, &c);
	if (!solve_quadratic(a, b, c, &t) || t >= t_max)
		return (false);
	set_sphere_hit(sp, ray, t, rec);
	return (true);
}
