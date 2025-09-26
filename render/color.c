#include "../minirt.h"

static double clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

int color_to_int(t_color color)
{
	int r;
	int g;
	int b;

	r = (int)clamp(color.x, 0.0, 255.0);
	g = (int)clamp(color.y, 0.0, 255.0);
	b = (int)clamp(color.z, 0.0, 255.0);
	return ((r << 16) | (g << 8) | b);
}

void my_mlx_pixel_put(t_mlx_data *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

t_color color_gamma(t_color c, double gamma)
{
	t_color out;
	double ig;

	if (gamma <= 0.0)
		return (c);
	ig = 1.0 / gamma;
	out.x = 255.0 * pow(clamp(c.x / 255.0, 0.0, 1.0), ig);
	out.y = 255.0 * pow(clamp(c.y / 255.0, 0.0, 1.0), ig);
	out.z = 255.0 * pow(clamp(c.z / 255.0, 0.0, 1.0), ig);
	return (out);
}