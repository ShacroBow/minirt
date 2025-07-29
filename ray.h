#ifndef RAY_H
# define RAY_H

# include "vec3.h"

typedef struct s_ray
{
	t_point3	origin;
	t_vec3		direction;
}	t_ray;

// Calculates the position at a point t along the ray
t_point3	ray_at(t_ray r, double t);
t_color		ray_color(t_ray r);


#endif