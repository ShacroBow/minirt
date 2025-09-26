#include "../minirt.h"

typedef struct s_cap
{
	t_point	center;
	t_vec3	normal;
	double	r2;
	int		invert;
} t_cap;

static bool	cap_hit(const t_cap *cap, const t_ray *ray, double *t,
			t_hit_record *rec)
{
	double	denom;
	double	t_cap;
	t_point	p;
	t_vec3	diff;

	denom = vec_dot(ray->direction, cap->normal);
	if (fabs(denom) <= EPSILON)
		return (false);
	t_cap = vec_dot(vec_sub(cap->center, ray->origin), cap->normal) / denom;
	if (t_cap <= EPSILON || t_cap >= *t)
		return (false);
	p = vec_add(ray->origin, vec_mult(ray->direction, t_cap));
	diff = vec_sub(p, cap->center);
	if (vec_length_squared(diff) >= cap->r2)
		return (false);
	*t = t_cap;
	if (cap->invert)
		rec->normal = vec_mult(cap->normal, -1);
	else
		rec->normal = cap->normal;
	return (true);
}

static bool	check_caps(const t_cylinder *cy, const t_ray *ray, double *t,
			t_hit_record *rec)
{
	t_cap	top;
	t_cap	bottom;
	bool	hit;

	top.center = vec_add(
		cy->center, vec_mult(cy->normal, cy->height / 2));
	top.normal = cy->normal;
	top.r2 = (cy->diameter * cy->diameter) / 4.0;
	top.invert = 0;
	bottom.center = vec_sub(
		cy->center, vec_mult(cy->normal, cy->height / 2));
	bottom.normal = cy->normal;
	bottom.r2 = top.r2;
	bottom.invert = 1;
	hit = false;
	if (cap_hit(&top, ray, t, rec))
		hit = true;
	if (cap_hit(&bottom, ray, t, rec))
		hit = true;
	return (hit);
}

static bool	solve_cy_quadratic(double a, double b, double c, double *t)
{
	double	discriminant;
	double	t0;
	double	t1;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (false);
	t0 = (-b - sqrt(discriminant)) / (2 * a);
	t1 = (-b + sqrt(discriminant)) / (2 * a);
	if (t0 > EPSILON && (t0 < t1 || t1 <= EPSILON))
		*t = t0;
	else if (t1 > EPSILON)
		*t = t1;
	else
		return (false);
	return (true);
}

static bool	side_hit(const t_cylinder *cy, const t_ray *ray, double t_max,
			t_hit_record *rec)
{
	t_vec3	oc;
	double	dn, on, t, y, a, b, c;

	oc = vec_sub(ray->origin, cy->center);
	dn = vec_dot(ray->direction, cy->normal);
	on = vec_dot(oc, cy->normal);
	t = t_max;
	a = vec_length_squared(ray->direction) - dn * dn;
	b = 2 * (vec_dot(ray->direction, oc) - dn * on);
	c = vec_length_squared(oc) - on * on
		- (cy->diameter * cy->diameter) / 4.0;
	if (!solve_cy_quadratic(a, b, c, &t) || t >= t_max)
		return (false);
	y = on + t * dn;
	if (fabs(y) > cy->height / 2)
		return (false);
	rec->t = t;
	rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
	rec->normal = vec_normalize(vec_sub(
		vec_sub(rec->point, cy->center), vec_mult(cy->normal, y)));
	if (vec_dot(ray->direction, rec->normal) > 0)
		rec->normal = vec_mult(rec->normal, -1);
	rec->front_face = true;
	return (true);
}

bool	hit_cylinder(const t_cylinder *cy, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	double	t;

	if (side_hit(cy, ray, t_max, rec))
		return (true);
	t = t_max;
	if (check_caps(cy, ray, &t, rec))
	{
		rec->t = t;
		rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
		rec->front_face = (vec_dot(ray->direction, rec->normal) < 0);
		return (true);
	}
	return (false);
}
