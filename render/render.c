#include "../minirt.h"

#define SAMPLES_PER_PIXEL 4

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
	int		s;
	t_ray	ray;
	t_color	pixel_color;
	t_color	sample_color;
	double	u;
	double	v;
	double	rand_x;
	double	rand_y;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			pixel_color = (t_color){0, 0, 0};
			s = 0;
			while (s < SAMPLES_PER_PIXEL)
			{
				rand_x = (double)rand() / (double)RAND_MAX;
				rand_y = (double)rand() / (double)RAND_MAX;
				u = ((double)x + rand_x) / (WIDTH - 1);
				v = ((double)y + rand_y) / (HEIGHT - 1);
				ray = create_ray(&prog->scene->camera, u, v);
				sample_color = trace_ray(&ray, prog->scene);
				pixel_color.x += sample_color.x;
				pixel_color.y += sample_color.y;
				pixel_color.z += sample_color.z;
				s++;
			}
			pixel_color.x /= SAMPLES_PER_PIXEL;
			pixel_color.y /= SAMPLES_PER_PIXEL;
			pixel_color.z /= SAMPLES_PER_PIXEL;
			my_mlx_pixel_put(&prog->mlx, x, y, color_to_int(pixel_color));
			x++;
		}
		y++;
	}
}