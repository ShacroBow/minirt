#include "../minirt.h"
#include "minirt.h"

t_vec3	vec_cross(t_vec3 v1, t_vec3 v2)
{
	t_vec3	result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}

double	vec_length_squared(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

double	vec_length(t_vec3 v)
{
	return (sqrt(vec_length_squared(v)));
}

t_vec3	vec_normalize(t_vec3 v)
{
	double	len;

	len = vec_length(v);
	if (len == 0)
		return (v);
	return (vec_div(v, len));
}

t_vec3	vec_reflect(t_vec3 v, t_vec3 n)
{
	return (vec_sub(v, vec_mult(n, 2 * vec_dot(v, n))));
}