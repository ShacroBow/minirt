#include "../include/minirt.h"

bool	quadratic(double a, double b, double c, double *t)
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

static bool	try_hit_object(const t_object *obj, const t_ray *ray,
			double t_max, t_hit_record *out)
{
	bool	hit;

	hit = false;
	if (obj->type == SPHERE)
		hit = hit_sphere(obj->shape_data, ray, t_max, out);
	else if (obj->type == PLANE)
		hit = hit_plane(obj->shape_data, ray, t_max, out);
	else if (obj->type == CYLINDER)
		hit = hit_cylinder(obj->shape_data, ray, t_max, out);
	else if (obj->type == CONE)
		hit = hit_cone(obj->shape_data, ray, t_max, out);
	if (hit && out)
	{
		out->color = obj->color;
		out->reflect = obj->reflectivity;
		out->transparency = obj->transparency;
		out->refractive_index = obj->refractive_index;
		out->has_checkerboard = obj->has_checkerboard;
		out->checker_color = obj->checker_color;
		out->type = obj->type;
		out->obj = (t_object *)obj;
	}
	return (hit);
}

bool	hit(const t_object *world, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	const t_object	*cur;
	bool			hit_any;
	double			closest;

	hit_any = false;
	closest = t_max;
	cur = world;
	while (cur)
	{
		if (try_hit_object(cur, ray, closest, rec))
		{
			hit_any = true;
			closest = rec->t;
		}
		cur = cur->next;
	}
	return (hit_any);
}

bool	hit_any(const t_object *world, const t_ray *ray, double t_max)
{
	const t_object	*cur;

	cur = world;
	while (cur)
	{
		if (try_hit_object(cur, ray, t_max, NULL))
		{
			if (cur->transparency < 1.0)
				return (true);
		}
		cur = cur->next;
	}
	return (false);
}

bool	hit_any_cached(const t_object *world, const t_ray *ray, \
			double t_max, const t_object **cache)
{
	const t_object	*cur;

	if (cache && *cache)
	{
		if (try_hit_object(*cache, ray, t_max, NULL))
		{
			if ((*cache)->transparency < 1.0)
				return (true);
		}
	}
	cur = world;
	while (cur)
	{
		if (try_hit_object(cur, ray, t_max, NULL))
		{
			if (cur->transparency < 1.0)
			{
				if (cache)
					*cache = cur;
				return (true);
			}
		}
		cur = cur->next;
	}
	return (false);
}
