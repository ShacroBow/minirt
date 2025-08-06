#include "../minirt.h"

static double	clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(255.999 * clamp(color.x / 255.0, 0.0, 1.0));
	g = (int)(255.999 * clamp(color.y / 255.0, 0.0, 1.0));
	b = (int)(255.999 * clamp(color.z / 255.0, 0.0, 1.0));
	return ((r << 16) | (g << 8) | b);
}

void	my_mlx_pixel_put(t_mlx_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

t_color	color_mult(t_color c1, t_color c2)
{
	t_color	result;

	result.x = (c1.x / 255.0) * c2.x;
	result.y = (c1.y / 255.0) * c2.y;
	result.z = (c1.z / 255.0) * c2.z;
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