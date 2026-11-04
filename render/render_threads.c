#include "../include/minirt.h"

__thread t_render_stats	*g_thread_stats = NULL;

static int	next_line(t_program *prog)
{
	int	line;

	pthread_mutex_lock(&prog->work_mutex);
	line = prog->next_line;
	prog->next_line += prog->pixel_step;
	pthread_mutex_unlock(&prog->work_mutex);
	return (line);
}

static void	commit_stats(t_program *prog, t_render_stats *stats)
{
	pthread_mutex_lock(&prog->stats_mutex);
	prog->ray_count += stats->ray_count;
	prog->shading_time += stats->shading_time;
	prog->intersect_time += stats->intersect_time;
	pthread_mutex_unlock(&prog->stats_mutex);
}

static void	render_row(t_thread_task *task, int y)
{
	int	x;

	if (task->ctx.mode != RENDER_CLASSIC)
	{
		progressive_row(&task->ctx, y, task->ctx.mode == RENDER_ACCUM);
		return ;
	}
	x = 0;
	while (x < WIDTH)
	{
		pixel_block(task->prog, x, y,
			color_to_int(render_pixel(&task->ctx, x, y)));
		x += task->prog->pixel_step;
	}
	if (DEBUG)
	{
		pthread_mutex_lock(&task->prog->stats_mutex);
		debug_progress(task->prog, y);
		pthread_mutex_unlock(&task->prog->stats_mutex);
	}
}

static void	*render_worker(void *arg)
{
	t_thread_task	*task;
	int				y;

	task = (t_thread_task *)arg;
	g_thread_stats = &task->stats;
	y = next_line(task->prog);
	while (y < task->prog->line_end)
	{
		render_row(task, y);
		y = next_line(task->prog);
	}
	commit_stats(task->prog, &task->stats);
	g_thread_stats = NULL;
	return (NULL);
}

/**
 * Renders rows first, first + pixel_step, ... (below end) on all threads.
 */
void	launch_render_threads(t_program *prog, t_render_ctx *ctx, int first,
			int end)
{
	pthread_t		threads[64];
	t_thread_task	tasks[64];
	int				i;

	prog->next_line = first;
	prog->line_end = end;
	if (prog->threads > 64)
		prog->threads = 64;
	if (prog->threads < 1)
		prog->threads = 1;
	i = 0;
	while (i < prog->threads)
	{
		tasks[i].prog = prog;
		tasks[i].ctx = *ctx;
		tasks[i].ctx.stats = &tasks[i].stats;
		ft_bzero(&tasks[i].stats, sizeof(t_render_stats));
		pthread_create(&threads[i], NULL, render_worker, &tasks[i]);
		i++;
	}
	i = 0;
	while (i < prog->threads)
		pthread_join(threads[i++], NULL);
}
