#include "../include/minirt.h"

t_color	trace_ray(const t_ray *ray, const t_scene *scene)
{
	t_hit_record	rec;

	if (hit(scene->objects, ray, INFINITY, &rec))
		return (phong_shading(&rec, scene, &ray->direction));
	return (bg_color_from_dir(scene, &ray->direction));
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
