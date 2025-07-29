#include "ray.h"

t_point3	ray_at(t_ray r, double t)
{
	return (vec3_add(r.origin, vec3_multiply_scalar(r.direction, t)));
}