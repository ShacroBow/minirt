#include "../minirt.h"

/* Color arithmetic helpers split from color.c to satisfy 42 Norm */

t_color	color_mult(t_color c1, t_color c2)
{
	t_color	result;
	double	inv_255;

	inv_255 = 0.00392156862;
	result.x = c1.x * c2.x * inv_255;
	result.y = c1.y * c2.y * inv_255;
	result.z = c1.z * c2.z * inv_255;
	return (result);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.x = c1.x + c2.x;
	result.y = c1.y + c2.y;
	result.z = c1.z + c2.z;
	return (result);
}

t_color	color_scale(t_color c, double scalar)
{
	t_color	result;

	result.x = c.x * scalar;
	result.y = c.y * scalar;
	result.z = c.z * scalar;
	return (result);
}
