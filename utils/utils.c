#include "../include/minirt.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	get_time_us(void)
{
	struct timeval	v;

	gettimeofday(&v, NULL);
	return (v.tv_sec * 1000000 + v.tv_usec);
}

void	update_render_stats(t_program *prog, long *t_start, bool is_shading)
{
	if (DEBUG)
	{
		if (is_shading)
			prog->shading_time += (get_time_us() - *t_start);
		else
			prog->intersect_time += (get_time_us() - *t_start);
		*t_start = get_time_us();
	}
}

t_color	handle_reflection(const t_ray *ray, t_hit_record *rec, \
	t_program *prog, int depth)
{
	t_ray	r_ray;
	t_color	reflect;

	r_ray.direction = vec_reflect(ray->direction, rec->normal);
	r_ray.origin = vec_add(rec->point, vec_mult(rec->normal, 0.001));
	reflect = trace_ray_recursive(&r_ray, prog, depth - 1);
	return (color_scale(reflect, rec->reflect));
}
