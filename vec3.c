#include "vec3.h"

// Adds two vectors
t_vec3	vec3_add(t_vec3 u, t_vec3 v)
{
	t_vec3	result;

	result.x = u.x + v.x;
	result.y = u.y + v.y;
	result.z = u.z + v.z;
	return (result);
}

// Subtracts vector v from u
t_vec3	vec3_subtract(t_vec3 u, t_vec3 v)
{
	t_vec3	result;

	result.x = u.x - v.x;
	result.y = u.y - v.y;
	result.z = u.z - v.z;
	return (result);
}

// Multiplies two vectors component-wise
t_vec3	vec3_multiply(t_vec3 u, t_vec3 v)
{
	t_vec3	result;

	result.x = u.x * v.x;
	result.y = u.y * v.y;
	result.z = u.z * v.z;
	return (result);
}

// Multiplies a vector by a scalar value
t_vec3	vec3_multiply_scalar(t_vec3 v, double t)
{
	t_vec3	result;

	result.x = v.x * t;
	result.y = v.y * t;
	result.z = v.z * t;
	return (result);
}

// Divides a vector by a scalar value
t_vec3	vec3_divide_scalar(t_vec3 v, double t)
{
	// Reusing the multiplication function is efficient and clear
	return (vec3_multiply_scalar(v, 1 / t));
}

// Calculates the squared length of a vector
double	vec3_length_squared(t_vec3 v)
{
	return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

// Calculates the length (magnitude) of a vector
double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}

// Add this entire function
t_vec3	vec3_unit(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len > 0)
		return (vec3_divide_scalar(v, len));
	return (v);
}