#ifndef VEC3_H
# define VEC3_H

# include <math.h>
# include <stdio.h>

// Struct definition for a 3D vector
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

// Type aliases for semantic clarity
typedef t_vec3	t_point3;
typedef t_vec3	t_color;

// Vector utility function prototypes
t_vec3	vec3_unit(t_vec3 v);
t_vec3	vec3_add(t_vec3 u, t_vec3 v);
t_vec3	vec3_subtract(t_vec3 u, t_vec3 v);
t_vec3	vec3_multiply(t_vec3 u, t_vec3 v);
t_vec3	vec3_multiply_scalar(t_vec3 v, double t);
t_vec3	vec3_divide_scalar(t_vec3 v, double t);
double	vec3_length_squared(t_vec3 v);
double	vec3_length(t_vec3 v);

#endif