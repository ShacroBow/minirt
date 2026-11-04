#include "../include/minirt.h"

double	fast_rand(unsigned long *seed)
{
	*seed = (*seed * 1103515245u + 12345u) & 0x7fffffff;
	return ((double)*seed / 2147483648.0);
}

t_ray	create_ray(const t_camera *cam, double u, double v)
{
	t_ray	r;
	t_vec3	horizontal_part;
	t_vec3	vertical_part;

	r.origin = cam->center;
	horizontal_part = vec_mult(cam->horizontal, u);
	vertical_part = vec_mult(cam->vertical, v);
	r.direction = vec_sub(vec_add(cam->lower_left_corner, \
				vec_add(horizontal_part, vertical_part)), r.origin);
	r.direction = vec_normalize(r.direction);
	return (r);
}
// normalize not necessary. r.direction = vec_normalize(r.direction);

t_color	get_aa_sample(t_render_ctx *ctx, int x, int y)
{
	t_color			sum;
	t_ray			ray;
	int				i;
	unsigned long	seed;

	sum = (t_color){0, 0, 0};
	seed = 123456789;
	i = 0;
	while (i < AA_SAMPLES)
	{
		ray = create_ray(&ctx->prog->scene->camera, \
						((x + ((i % GRID) + fast_rand(&seed)) * INV_GRID) * \
						ctx->inv_w), ((y + (((i / GRID) % GRID) + \
						fast_rand(&seed)) * INV_GRID) * ctx->inv_h));
		sum = color_add(sum, trace_ray(&ray, ctx->prog));
		i++;
	}
	return (color_scale(sum, ctx->inv_aa_samples));
}

t_color	render_pixel(t_render_ctx *ctx, int x, int y)
{
	t_color	sum;
	t_ray	ray;
	double	u;
	double	v;

	if (ctx->prog->aa_enabled)
		sum = get_aa_sample(ctx, x, y);
	else
	{
		u = ((double)x + 0.5) * ctx->inv_w;
		v = ((double)y + 0.5) * ctx->inv_h;
		ray = create_ray(&ctx->prog->scene->camera, u, v);
		sum = trace_ray(&ray, ctx->prog);
	}
	if (ENABLE_GAMMA)
		sum = color_gamma(sum, DISPLAY_GAMMA);
	return (sum);
}

void	init_render_utils(t_program *prog, t_render_ctx *ctx)
{
	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	ctx->prog = prog;
	ctx->inv_w = 1.0 / (double)WIDTH;
	ctx->inv_h = 1.0 / (double)HEIGHT;
	ctx->inv_aa_samples = 1.0 / (double)AA_SAMPLES;
	ctx->stats = NULL;
	if (DEBUG)
	{
		prog->ray_count = 0;
		prog->shading_time = 0;
		prog->intersect_time = 0;
	}
}
