#include "../minirt.h"

t_color	trace_ray(const t_ray *ray, const t_scene *scene)
{
	t_hit_record	rec;

	if (hit(scene->objects, ray, INFINITY, &rec))
		return (phong_shading(&rec, scene));
	return ((t_color){0, 0, 0});
}

static void	render_progress_update(int y)
{
	int	percent;

	percent = (int)(((long long)(y + 1) * 100) / HEIGHT);
	fprintf(stdout, "Rendering... %d%%\r", percent);
	fflush(stdout);
}

static void	render_row(t_program *prog, int y, double inv_w, double inv_h)
{
	int				x;
	t_color			c;

	x = 0;
	while (x < WIDTH)
	{
		c = get_pixel_color(prog, x, y, inv_w, inv_h);
		my_mlx_pixel_put(&prog->mlx, x, y, color_to_int(c));
		x++;
	}
}

void	render(t_program *prog)
{
	int		y;
	double	inv_w;
	double	inv_h;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	inv_w = 1.0 / (double)WIDTH;
	inv_h = 1.0 / (double)HEIGHT;
	fprintf(stdout, "Rendering... 0%%\r");
	fflush(stdout);
	y = 0;
	while (y < HEIGHT)
	{
		render_row(prog, y, inv_w, inv_h);
		render_progress_update(y);
		y++;
	}
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
		prog->mlx.img_ptr, 0, 0);
	fprintf(stdout, "Done rendering.       \n");
	fflush(stdout);
}