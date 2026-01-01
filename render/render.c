#include "../include/minirt.h"

static void	debug_progress(t_program *prog, int y)
{
	long		current_time;
	long		elapsed_ms;

	if (y == 0)
		prog->render_start_time = get_time_us();
	if (y % 5 == 0 || y == HEIGHT - 1)
	{
		current_time = get_time_us();
		elapsed_ms = (current_time - prog->render_start_time) / 1000;
		printf("\r[DEBUG] Progress: %2d%% | Rays: %7ld | Total: %4ldms | "\
			"Shading: %3ldms | Intersect: %3ldms", (y * 100) / HEIGHT,
			prog->ray_count, elapsed_ms, prog->shading_time / 1000,
			prog->intersect_time / 1000);
	}
	if (y == HEIGHT - 1)
		printf("\n");
}

t_color	trace_ray_recursive(const t_ray *ray, t_program *prog, \
	int depth)
{
	t_hit_record	rec;
	t_color			local;
	long			t_start;

	if (DEBUG)
		prog->ray_count++;
	if (DEBUG)
		t_start = get_time_us();
	if (hit(prog->scene->objects, ray, INFINITY, &rec))
	{
		update_render_stats(prog, &t_start, false);
		apply_bump(&rec, ray);
		local = phong_shading(&rec, prog->scene, &ray->direction);
		update_render_stats(prog, &t_start, true);
		if (depth > 0 && rec.reflect > 0.0)
			return (color_add(local, \
				handle_reflection(ray, &rec, prog, depth)));
		return (local);
	}
	update_render_stats(prog, &t_start, false);
	return (bg_color_from_dir(prog->scene, &ray->direction));
}

static void	pixel_block(t_program *prog, int x, int y, int color)
{
	int		i;
	int		j;
	char	*dst;

	j = 0;
	while (j < prog->pixel_step && (y + j) < HEIGHT)
	{
		i = 0;
		while (i < prog->pixel_step && (x + i) < WIDTH)
		{
			dst = prog->mlx.addr + (y + j) * prog->mlx.line_length + \
				(x + i) * (prog->mlx.bits_per_pixel / 8);
			*(unsigned int *)dst = color;
			i++;
		}
		j++;
	}
}

t_color	trace_ray(const t_ray *ray, t_program *prog)
{
	if (ENABLE_REFLECTIONS)
		return (trace_ray_recursive(ray, prog, MAX_REFLECTION_DEPTH));
	return (trace_ray_recursive(ray, prog, 0));
}

void	render(t_program *prog)
{
	int				x;
	int				y;
	t_render_ctx	ctx;

	init_render_utils(prog, &ctx);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		if (DEBUG)
			debug_progress(prog, y);
		while (x < WIDTH)
		{
			pixel_block(prog, x, y, color_to_int(render_pixel(&ctx, x, y)));
			x += prog->pixel_step;
		}
		y += prog->pixel_step;
	}
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
							prog->mlx.img_ptr, 0, 0);
}
