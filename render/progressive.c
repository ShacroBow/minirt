#include "../include/minirt.h"

void	init_threading(t_program *prog)
{
	int	count;

	count = (int)sysconf(_SC_NPROCESSORS_ONLN);
	if (count < 1)
		count = 1;
	prog->threads = count;
	pthread_mutex_init(&prog->work_mutex, NULL);
	pthread_mutex_init(&prog->stats_mutex, NULL);
}

void	progressive_init(t_program *prog)
{
	prog->accum = malloc(sizeof(t_color) * WIDTH * HEIGHT);
	prog->row_samples = ft_calloc(HEIGHT, sizeof(int));
	if (!prog->accum || !prog->row_samples)
		exit_cleanup(prog, "Error: Allocation failure.");
	prog->band = prog->threads * 2;
	progressive_reset(prog);
}

/**
 * Something changed: forget the samples so far and show a quick preview.
 */
void	progressive_reset(t_program *prog)
{
	ft_bzero(prog->row_samples, sizeof(int) * HEIGHT);
	prog->scene->sample = 0;
	prog->pass_row = 0;
	prog->preview = true;
}

/**
 * mlx loop hook: refines the image a little on every call, then idles once
 * PROGRESSIVE_MAX_SAMPLES samples per pixel are reached.
 */
int	progressive_hook(void *param)
{
	t_program	*prog;

	prog = (t_program *)param;
	if (prog->preview)
		render_preview(prog);
	else if (prog->scene->sample < PROGRESSIVE_MAX_SAMPLES)
		render_band(prog);
	else
	{
		usleep(10000);
		return (0);
	}
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
		prog->mlx.img_ptr, 0, 0);
	return (0);
}

/**
 * P: one full quality render with antialiasing, then no more refining
 * until something changes.
 */
void	render_final(t_program *prog)
{
	int	step;

	prog->aa_enabled = 1;
	step = prog->pixel_step;
	prog->pixel_step = 1;
	render(prog);
	prog->pixel_step = step;
	prog->aa_enabled = 0;
	prog->scene->sample = PROGRESSIVE_MAX_SAMPLES;
}
