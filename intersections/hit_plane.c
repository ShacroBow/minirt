#include "../minirt.h"

bool	hit_plane(const t_plane *pl, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	double	denom;
	double	t;
	t_vec3	p0_to_o;

	denom = vec_dot(ray->direction, pl->normal);
	if (fabs(denom) < EPSILON)
		return (false);
	p0_to_o = vec_sub(pl->point, ray->origin);
	t = vec_dot(p0_to_o, pl->normal) / denom;
	if (t < EPSILON |

| t >= t_max)
		return (false);
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
	return (true);
}