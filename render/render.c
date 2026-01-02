#include "../include/minirt.h"

void	debug_progress(t_program *prog, int y)
{
	long		current_time;
	long		elapsed_ms;
	static long	every_while = (HEIGHT / 100) * 5;

	if (y == 0)
		prog->render_start_time = get_time_us();
	if ((y % every_while == 0) || y == HEIGHT - 1 || y == 0)
	{
		current_time = get_time_us();
		elapsed_ms = (current_time - prog->render_start_time) / 1000;
		printf("\r[DEBUG] Progress: %2d%% | Rays: %7ld | Total: %4ldms | "\
			"Shading: %3ldms | Intersect: %3ldms", (y * 100) / HEIGHT,
			prog->ray_count, elapsed_ms, prog->shading_time / 1000,
			prog->intersect_time / 1000);
		fflush(stdout); // needed when program slow.
	}
	if (y + prog->pixel_step >= HEIGHT)
		printf("\n");
}

t_color	trace_ray_recursive(const t_ray *ray, t_program *prog, \
	int depth)
{
	t_hit_record	rec;
	t_color			local;
	long			t_start;

	if (DEBUG && g_thread_stats)
		g_thread_stats->ray_count++;
	if (DEBUG)
		t_start = get_time_us();
	if (hit(prog->scene->objects, ray, INFINITY, &rec))
	{
		update_render_stats(prog, &t_start, false);
		apply_bump(&rec, ray);
		local = phong_shading(&rec, prog->scene, &ray->direction);
		update_render_stats(prog, &t_start, true);
		if (depth > 0)
		{
			if (ENABLE_TRANSPARENCY && rec.transparency > 0.0)
				local = color_add(color_add(color_scale(handle_transparency(ray, &rec, \
					prog, depth), rec.transparency), \
					color_scale(local, 1.0 - rec.transparency)), \
					get_specular_only(&rec, prog->scene, &ray->direction));
			if (ENABLE_REFLECTIONS && rec.reflect > 0.0)
				local = color_add(local, \
					handle_reflection(ray, &rec, prog, depth));
		}
		return (local);
	}
	update_render_stats(prog, &t_start, false);
	return (bg_color_from_dir(prog->scene, &ray->direction));
}

void	pixel_block(t_program *prog, int x, int y, int color)
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
			dst = prog->mlx.addr + (ptrdiff_t)(y + j) * prog->mlx.line_length + \
				(ptrdiff_t)(x + i) * (prog->mlx.bits_per_pixel / 8);
			*(unsigned int *)dst = color;
			i++;
		}
		j++;
	}
}

t_color	trace_ray(const t_ray *ray, t_program *prog)
{
	int	depth;

	depth = 0;
	if ((ENABLE_REFLECTIONS || ENABLE_TRANSPARENCY) && MAX_DEPTH > depth)
		depth = MAX_DEPTH;
	return (trace_ray_recursive(ray, prog, depth));
}

void	render(t_program *prog)
{
	t_render_ctx	ctx;

	init_render_utils(prog, &ctx);
	if (DEBUG)
		prog->render_start_time = get_time_us();
	launch_render_threads(prog, &ctx);
	if (DEBUG)
		debug_progress(prog, HEIGHT - 1);
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
								prog->mlx.img_ptr, 0, 0);
}
