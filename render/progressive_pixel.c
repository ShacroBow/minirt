#include "../include/minirt.h"

/**
 * One ray through a random point of the pixel (block): averaging many of
 * them gives antialiasing for free.
 */
static t_color	jittered_sample(t_render_ctx *ctx, int x, int y)
{
	unsigned long	seed;
	t_ray			ray;
	double			step;

	seed = pixel_seed(x, y, ctx->prog->scene->sample);
	step = (double)ctx->prog->pixel_step;
	ray = create_ray(&ctx->prog->scene->camera,
			(x + fast_rand(&seed) * step) * ctx->inv_w,
			(y + fast_rand(&seed) * step) * ctx->inv_h);
	return (trace_ray(&ray, ctx->prog));
}

static t_color	display_color(t_color c)
{
	if (ENABLE_GAMMA)
		return (color_gamma(c, DISPLAY_GAMMA));
	return (c);
}

/**
 * Traces one sample for every pixel of row y. With accumulate, the sample
 * is added to prog->accum and the row shows the average so far.
 */
void	progressive_row(t_render_ctx *ctx, int y, bool accumulate)
{
	t_program	*prog;
	t_color		c;
	size_t		i;
	int			n;
	int			x;

	prog = ctx->prog;
	n = prog->row_samples[y];
	x = 0;
	while (x < WIDTH)
	{
		c = jittered_sample(ctx, x, y);
		if (accumulate)
		{
			i = (size_t)y * WIDTH + x;
			if (n > 0)
				c = color_add(prog->accum[i], c);
			prog->accum[i] = c;
			c = color_scale(c, 1.0 / (n + 1));
		}
		pixel_block(prog, x, y, color_to_int(display_color(c)));
		x += prog->pixel_step;
	}
	if (accumulate)
		prog->row_samples[y] = n + 1;
}
