#include "../include/minirt.h"

t_vec3	cone_coeffs(const t_cone *cy, const t_ray *ray)
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
