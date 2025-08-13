#include "../minirt.h"

static bool	check_caps(const t_cylinder *cy, const t_ray *ray, double *t,
		t_hit_record *rec)
{
	double	denom;
	double	t_cap;
	t_point	p;
	t_vec3	p_to_center;
	bool	hit;

	hit = false;
	denom = vec_dot(ray->direction, cy->normal);
	if (fabs(denom) > EPSILON)
	{
		t_cap = vec_dot(vec_sub(vec_add(cy->center, vec_mult(cy->normal,
							cy->height / 2)), ray->origin), cy->normal) / denom;
		p = vec_add(ray->origin, vec_mult(ray->direction, t_cap));
		p_to_center = vec_sub(p, vec_add(cy->center,
					vec_mult(cy->normal, cy->height / 2)));
		if (t_cap > EPSILON && t_cap < *t
			&& vec_length_squared(p_to_center) < pow(cy->diameter / 2, 2))
		{
			*t = t_cap;
			rec->normal = cy->normal;
			hit = true;
		}
		t_cap = vec_dot(vec_sub(vec_sub(cy->center, vec_mult(cy->normal,
							cy->height / 2)), ray->origin), cy->normal) / denom;
		p = vec_add(ray->origin, vec_mult(ray->direction, t_cap));
		p_to_center = vec_sub(p, vec_sub(cy->center,
					vec_mult(cy->normal, cy->height / 2)));
		if (t_cap > EPSILON && t_cap < *t
			&& vec_length_squared(p_to_center) < pow(cy->diameter / 2, 2))
		{
			*t = t_cap;
			rec->normal = vec_mult(cy->normal, -1);
			hit = true;
		}
	}
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

bool	hit_cylinder(const t_cylinder *cy, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	t;
	double	y;

	oc = vec_sub(ray->origin, cy->center);
	a = vec_length_squared(ray->direction) - pow(vec_dot(ray->direction,
				cy->normal), 2);
	b = 2 * (vec_dot(ray->direction, oc) - vec_dot(ray->direction, cy->normal)
			* vec_dot(oc, cy->normal));
	c = vec_length_squared(oc) - pow(vec_dot(oc, cy->normal), 2)
		- pow(cy->diameter / 2, 2);
	t = t_max;
	if (solve_cy_quadratic(a, b, c, &t) && t < t_max)
	{
		y = vec_dot(oc, cy->normal) + t * vec_dot(ray->direction, cy->normal);
		if (fabs(y) <= cy->height / 2)
		{
			rec->t = t;
			rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
			rec->normal = vec_normalize(vec_sub(vec_sub(rec->point, cy->center),
						vec_mult(cy->normal, y)));
			if (vec_dot(ray->direction, rec->normal) > 0)
				rec->normal = vec_mult(rec->normal, -1);
			rec->front_face = true;
			return (true);
		}
	}
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
