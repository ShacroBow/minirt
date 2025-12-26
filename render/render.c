#include "../include/minirt.h"

static t_color	trace_ray_recursive(const t_ray *ray, const t_scene *scene, \
	int depth)
{
	t_hit_record	rec;
	t_color			local;
	t_color			reflect;
	t_ray			r_ray;

	if (hit(scene->objects, ray, INFINITY, &rec))
	{
		local = phong_shading(&rec, scene, &ray->direction);
		if (depth > 0 && rec.reflect > 0.0)
		{
			r_ray.direction = vec_reflect(vec_normalize(ray->direction), \
				rec.normal);
			r_ray.origin = vec_add(rec.point, vec_mult(rec.normal, 0.001));
			reflect = trace_ray_recursive(&r_ray, scene, depth - 1);
			return (color_add(local, color_scale(reflect, rec.reflect)));
		}
		return (local);
	}
	return (bg_color_from_dir(scene, &ray->direction));
}

t_color	trace_ray(const t_ray *ray, const t_scene *scene)
{
	if (ENABLE_REFLECTIONS)
		return (trace_ray_recursive(ray, scene, MAX_REFLECTION_DEPTH));
	return (trace_ray_recursive(ray, scene, 0));
}

void	render(t_program *prog)
{
	int				x;
	int				y;
	t_render_ctx	ctx;
	char			*row_ptr;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	ctx.prog = prog;
	ctx.inv_w = 1.0 / (double)WIDTH;
	ctx.inv_h = 1.0 / (double)HEIGHT;
	ctx.inv_aa_samples = 1.0 / (double)AA_SAMPLES;
	y = 0;
	while (y < HEIGHT)
	{
		row_ptr = prog->mlx.addr + y * prog->mlx.line_length;
		x = 0;
		while (x < WIDTH)
		{
			*(unsigned int *)row_ptr = color_to_int(render_pixel(&ctx, x, y));
			row_ptr += prog->mlx.bits_per_pixel / 8;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
							prog->mlx.img_ptr, 0, 0);
}
