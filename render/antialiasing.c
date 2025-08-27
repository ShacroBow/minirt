#include "../minirt.h"

#if ENABLE_AA

static void	calculate_aa_sample_coords(int s, int grid, double *ox, double *oy)
{
	int	i;
	int	j;

	i = s % grid;
	j = s / grid;
	*ox = ((double)i + (double)rand() / (double)RAND_MAX) / (double)grid;
	*oy = ((double)j + (double)rand() / (double)RAND_MAX) / (double)grid;
}

static t_ray	get_aa_ray(t_program *prog, t_render_params *params,
	int s, int grid)
{
	double	ox;
	double	oy;
	double	u;
	double	v;

	calculate_aa_sample_coords(s, grid, &ox, &oy);
	u = ((double)params->x + ox) * params->inv_w;
	v = ((double)params->y + oy) * params->inv_h;
	if (u >= 1.0)
		u = 1.0 - 1e-9;
	if (v >= 1.0)
		v = 1.0 - 1e-9;
	return (create_ray(&prog->scene->camera, u, v));
}

t_color	render_aa_sample(t_program *prog, t_render_params *params)
{
	int		s;
	int		grid;
	t_ray	ray;
	t_color	sum;

	sum = (t_color){0, 0, 0};
	grid = (int)ceil(sqrt((double)AA_SAMPLES));
	s = 0;
	while (s < AA_SAMPLES)
	{
		ray = get_aa_ray(prog, params, s, grid);
		sum = color_add(sum, trace_ray(&ray, prog->scene));
		s++;
	}
	return (color_scale(sum, 1.0 / (double)AA_SAMPLES));
}

#endif
