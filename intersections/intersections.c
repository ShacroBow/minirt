#include "../include/minirt.h"

static bool	try_hit_object(const t_object *obj, const t_ray *ray,
			double t_max, t_hit_record *out)
{
	t_hit_record	tmp;

	if (obj->type == SPHERE && hit_sphere(obj->shape_data, ray, t_max, &tmp))
	{
		*out = tmp;
		out->color = obj->color;
		out->reflect = obj->reflectivity;
		return (true);
	}
	if (obj->type == PLANE && hit_plane(obj->shape_data, ray, t_max, &tmp))
	{
		*out = tmp;
		out->color = obj->color;
		out->reflect = obj->reflectivity;
		return (true);
	}
	if (obj->type == CYLINDER
		&& hit_cylinder(obj->shape_data, ray, t_max, &tmp))
	{
		*out = tmp;
		out->color = obj->color;
		out->reflect = obj->reflectivity;
		return (true);
	}
	return (false);
}

bool	hit(const t_object *world, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	const t_object	*cur;
	t_hit_record	best;
	bool			hit_any;
	double			closest;

	hit_any = false;
	closest = t_max;
	cur = world;
	while (cur)
	{
		if (try_hit_object(cur, ray, closest, &best))
		{
			hit_any = true;
			closest = best.t;
			*rec = best;
		}
		cur = cur->next;
	}
	return (hit_any);
}

bool	hit_any(const t_object *world, const t_ray *ray, double t_max)
{
	const t_object	*cur;
	t_hit_record	temp;

	cur = world;
	while (cur)
	{
		if (try_hit_object(cur, ray, t_max, &temp))
			return (true);
		cur = cur->next;
	}
	return (false);
}
