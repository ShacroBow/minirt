#include "../minirt.h"

#ifndef AA_SAMPLES
# define AA_SAMPLES 1
#endif

t_color	trace_ray(const t_ray *ray, const t_scene *scene)
{
	t_hit_record	rec;

	if (hit(scene->objects, ray, INFINITY, &rec))
		return (phong_shading(&rec, scene));
	return ((t_color){0, 0, 0});
}

static t_color	render_pixel(t_program *prog, int x, int y, double inv_w, double inv_h)
{
	double	u;
	double	v;
	t_ray	ray;
	t_color	sum;
	int		i;

#if ENABLE_AA && AA_SAMPLES == 4
	sum = (t_color){0, 0, 0};
	i = 0;
	while (i < 4)
	{
		u = ((double)x + ((i & 1) ? 0.75 : 0.25)) * inv_w;
		v = ((double)y + ((i < 2) ? 0.25 : 0.75)) * inv_h;
		ray = create_ray(&prog->scene->camera, u, v);
		sum = color_add(sum, trace_ray(&ray, prog->scene));
		i++;
	}
	sum = color_scale(sum, 0.25);
#else
	u = ((double)x + 0.5) * inv_w;
	v = ((double)y + 0.5) * inv_h;
	ray = create_ray(&prog->scene->camera, u, v);
	sum = trace_ray(&ray, prog->scene);
#endif
#if ENABLE_GAMMA
	sum = color_gamma(sum, DISPLAY_GAMMA);
#endif
	return (sum);
}

void	render(t_program *prog)
{
	int		x;
	int		y;
	double	inv_w;
	double	inv_h;
	t_color	c;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	inv_w = 1.0 / (WIDTH - 1);
	inv_h = 1.0 / (HEIGHT - 1);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			c = render_pixel(prog, x, y, inv_w, inv_h);
			my_mlx_pixel_put(&prog->mlx, x, y, color_to_int(c));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
		prog->mlx.img_ptr, 0, 0);
}