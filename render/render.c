#include "../minirt.h"

t_color trace_ray(const t_ray *ray, const t_scene *scene)
{
	t_hit_record rec;

	if (hit(scene->objects, ray, INFINITY, &rec))
		return (phong_shading(&rec, scene, &ray->direction));
	return (bg_color_from_dir(scene, &ray->direction));
}

static t_color render_pixel(t_program *prog, int x, int y, double inv_w, double inv_h)
{
	double u;
	double v;
	t_ray ray;
	t_color sum;

#if ENABLE_AA
	sum = (t_color){0, 0, 0};
	int s;
	int grid;
	int i;
	int j;
	double ox;
	double oy;
	grid = (int)ceil(sqrt((double)AA_SAMPLES));
	s = 0;
	while (s < AA_SAMPLES)
	{
		i = s % grid;
		j = s / grid;
		ox = ((double)i + (double)rand() / (double)RAND_MAX) / (double)grid;
		oy = ((double)j + (double)rand() / (double)RAND_MAX) / (double)grid;
		u = ((double)x + ox) * inv_w;
		v = ((double)y + oy) * inv_h;
		if (u >= 1.0)
			u = 1.0 - 1e-9;
		if (v >= 1.0)
			v = 1.0 - 1e-9;
		ray = create_ray(&prog->scene->camera, u, v);
		sum = color_add(sum, trace_ray(&ray, prog->scene));
		s++;
	}
	sum = color_scale(sum, 1.0 / (double)AA_SAMPLES);
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

void render(t_program *prog)
{
	int x;
	int y;
	double inv_w;
	double inv_h;
	t_color c;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	inv_w = 1.0 / (double)WIDTH;
	inv_h = 1.0 / (double)HEIGHT;
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