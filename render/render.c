#include "../minirt.h"

#define GRID 3
#define INV_GRID 0.3333333333

static inline double	fast_rand(unsigned long *seed)
{
	*seed = (*seed * 1103515245u + 12345u) & 0x7fffffff;
	return ((double)*seed / 2147483648.0);
}

t_color	trace_ray(const t_ray *ray, const t_scene *scene)
{
	t_hit_record	rec;

	if (hit(scene->objects, ray, INFINITY, &rec))
		return (phong_shading(&rec, scene, &ray->direction));
	return (bg_color_from_dir(scene, &ray->direction));
}

static t_color	get_aa_sample(t_program *prog, int x, int y,
		double iw, double ih, double inv_samples)
{
	t_color			sum;
	t_ray			ray;
	int				sx;
	int				sy;
	int				i;
	unsigned long	seed;
	t_scene			*scene;

	scene = prog->scene;
	sum = (t_color){0, 0, 0};
	seed = 123456789;
	i = 0;
	while (i < AA_SAMPLES)
	{
		sx = i % GRID;
		sy = (i / GRID) % GRID;
		ray = create_ray(&scene->camera,
				((x + (sx + fast_rand(&seed)) * INV_GRID) * iw),
				((y + (sy + fast_rand(&seed)) * INV_GRID) * ih));
		sum = color_add(sum, trace_ray(&ray, scene));
		i++;
	}
	return (color_scale(sum, inv_samples));
}

static t_color	render_pixel(t_program *prog, int x, int y,
		double inv_w, double inv_h, double inv_aa_samples)
{
	t_color	sum;
	t_ray	ray;
	double	u;
	double	v;

	if (prog->aa_enabled)
		sum = get_aa_sample(prog, x, y, inv_w, inv_h, inv_aa_samples);
	else
	{
		u = ((double)x + 0.5) * inv_w;
		v = ((double)y + 0.5) * inv_h;
		ray = create_ray(&prog->scene->camera, u, v);
		sum = trace_ray(&ray, prog->scene);
	}
	if (ENABLE_GAMMA)
		sum = color_gamma(sum, DISPLAY_GAMMA);
	return (sum);
}

void	render(t_program *prog)
{
	int			x;
	int			y;
	double		inv_w;
	double		inv_h;
	double		inv_aa_samples;
	t_color		c;
	char		*row_ptr;
	int			bpp_bytes;
	t_scene		*scene;

	scene = prog->scene;
	setup_camera(&scene->camera, (double)WIDTH / (double)HEIGHT);
	inv_w = 1.0 / (double)WIDTH;
	inv_h = 1.0 / (double)HEIGHT;
	inv_aa_samples = 1.0 / (double)AA_SAMPLES;
	bpp_bytes = prog->mlx.bits_per_pixel / 8;
	y = 0;
	while (y < HEIGHT)
	{
		row_ptr = prog->mlx.addr + y * prog->mlx.line_length;
		{
			char *p = row_ptr;
			x = 0;
			while (x < WIDTH)
			{
				c = render_pixel(prog, x, y, inv_w, inv_h, inv_aa_samples);
				*(unsigned int *)p = color_to_int(c);
				p += bpp_bytes;
				x++;
			}
		}
		y++;
	}
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
							prog->mlx.img_ptr, 0, 0);
}
