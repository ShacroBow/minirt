#include "minirt.h" // Or whichever header has your other includes
#include "vec3.h"
#include "ray.h"

// Returns a sky-blue gradient color based on the ray's y-direction.
t_color	ray_color(t_ray r)
{
	t_vec3	unit_direction;
	double	t;

	unit_direction = vec3_unit(r.direction);
	t = 0.5 * (unit_direction.y + 1.0);

	// Linearly blend between white (t=1.0) and blue (t=0.0)
	return (vec3_add(vec3_multiply_scalar((t_color){1.0, 1.0, 1.0}, 1.0 - t),
					vec3_multiply_scalar((t_color){0.5, 0.7, 1.0}, t)));
}

// t_color	ray_color(t_ray r)
// {
// 	t_vec3	unit_direction = vec3_unit(r.direction);
// 	double	t = 0.5 * (unit_direction.y + 1.0);
// 	t_color white = {1.0, 1.0, 1.0};
// 	t_color blue = {0.5, 0.7, 1.0};
// 	t_vec3 part1 = vec3_multiply_scalar(white, 1.0 - t);
// 	t_vec3 part2 = vec3_multiply_scalar(blue, t);
// 	return (vec3_add(part1, part2));
// }