#include "../include/minirt.h"

static void	render_rows(t_program *prog, t_render_mode mode, int first,
				int end)
{
	t_render_ctx	ctx;

	init_render_utils(prog, &ctx);
	ctx.mode = mode;
	prog->scene->progressive = true;
	launch_render_threads(prog, &ctx, first, end);
}

/**
 * Whole frame, one sample per PREVIEW_STEP x PREVIEW_STEP block, not kept.
 */
void	render_preview(t_program *prog)
{
	int	step;

	step = prog->pixel_step;
	if (prog->pixel_step < PREVIEW_STEP)
		prog->pixel_step = PREVIEW_STEP;
	render_rows(prog, RENDER_PREVIEW, 0, HEIGHT);
	prog->pixel_step = step;
	prog->preview = false;
}

/**
 * Grows or shrinks the band of rows so one call takes about
 * FRAME_BUDGET_MS, but always gives every thread a row.
 */
static void	adapt_band(t_program *prog, long ms)
{
	int	min;

	min = prog->threads * prog->pixel_step;
	if (ms < FRAME_BUDGET_MS / 2 && prog->band < HEIGHT)
		prog->band *= 2;
	else if (ms > FRAME_BUDGET_MS && prog->band > min)
		prog->band /= 2;
	if (prog->band < min)
		prog->band = min;
	prog->band -= prog->band % prog->pixel_step;
}

static void	finish_pass(t_program *prog)
{
	prog->pass_row = 0;
	prog->scene->sample++;
	if (DEBUG)
	{
		printf("\rSamples per pixel: %4d | last pass: %5ld ms   ",
			prog->scene->sample, get_time_ms() - prog->pass_start);
		fflush(stdout);
	}
}

/**
 * Adds one sample to the next band of rows of the current pass.
 */
void	render_band(t_program *prog)
{
	long	start;
	int		end;

	if (prog->pass_row == 0)
		prog->pass_start = get_time_ms();
	end = prog->pass_row + prog->band;
	if (end > HEIGHT)
		end = HEIGHT;
	start = get_time_ms();
	render_rows(prog, RENDER_ACCUM, prog->pass_row, end);
	adapt_band(prog, get_time_ms() - start);
	prog->pass_row = end;
	if (prog->pass_row >= HEIGHT)
		finish_pass(prog);
}
