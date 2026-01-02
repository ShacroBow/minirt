#include "../include/minirt.h"

__thread t_render_stats	*g_thread_stats = NULL;

typedef struct s_thread_task
{
	t_program		*prog;
	t_render_ctx	ctx;
	t_render_stats	stats;
}	t_thread_task;

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

static void	*render_worker(void *arg)
{
	t_thread_task	*task; int y; int x; int color;
	task = (t_thread_task *)arg;
	g_thread_stats = &task->stats;
	while ((y = next_line(task->prog)) < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			color = color_to_int(render_pixel(&task->ctx, x, y));
			pixel_block(task->prog, x, y, color);
			x += task->prog->pixel_step;
		}
		if (DEBUG)
		{
			pthread_mutex_lock(&task->prog->stats_mutex);
			debug_progress(task->prog, y);
			pthread_mutex_unlock(&task->prog->stats_mutex);
		}
	}
	commit_stats(task->prog, &task->stats);
	g_thread_stats = NULL;
	return (NULL);
}

void	launch_render_threads(t_program *prog, t_render_ctx *ctx)
{
	pthread_t		threads[64];
	t_thread_task	tasks[64];
	int				i;
	int				total;

	prog->next_line = 0;
	total = prog->threads;
	if (total < 1)
		total = 1;
	if (total > 64)
		total = 64;
	prog->threads = total;
	for (i = 0; i < total; i++)
	{
		tasks[i].prog = prog;
		tasks[i].ctx = *ctx;
		tasks[i].ctx.stats = &tasks[i].stats;
		ft_bzero(&tasks[i].stats, sizeof(t_render_stats));
		pthread_create(&threads[i], NULL, render_worker, &tasks[i]);
	}
	for (i = 0; i < total; i++)
		pthread_join(threads[i], NULL);
}

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
