#include "../minirt.h"

t_color	trace_ray(const t_ray *ray, const t_scene *scene)
{
	t_hit_record	rec;

	if (hit(scene->objects, ray, INFINITY, &rec))
	{
		return (phong_shading(&rec, scene));
	}
	return ((t_color){0, 0, 0});
}

void	render(t_program *prog)
{
	int		x;
	int		y;
	t_ray	ray;
	t_color	pixel_color;
	double	u;
	double	v;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			u = (double)x / (WIDTH - 1);
			v = (double)y / (HEIGHT - 1);
			ray = create_ray(&prog->scene->camera, u, v);
			pixel_color = trace_ray(&ray, prog->scene);
			my_mlx_pixel_put(&prog->mlx, x, HEIGHT - 1 - y,
				color_to_int(pixel_color));
			x++;
		}
		y++;
	}
}