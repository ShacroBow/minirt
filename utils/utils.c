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

void	print_objects_status(const t_scene *scene)
{
	const t_object *cur;
	int i;

	if (!scene)
		return;
	cur = scene->objects;
	i = 0;
	printf("Scene objects status:\n");
	while (cur)
	{
		const char *type_s;
		switch (cur->type)
		{
			case SPHERE: type_s = "SPHERE"; break;
			case PLANE: type_s = "PLANE"; break;
			case CYLINDER: type_s = "CYLINDER"; break;
			case CONE: type_s = "CONE"; break;
			default: type_s = "UNKNOWN"; break;
		}
		printf("- Object %d: %s | has_texture=%d | has_bump=%d | bump_enabled=%d\n", i, type_s, cur->has_texture, cur->has_bump, cur->bump_enabled);
		if (cur->has_texture && cur->texture)
			printf("  texture: %dx%d channels=%d\n", cur->texture->width, cur->texture->height, cur->texture->channels);
		if (cur->has_bump && cur->bump)
			printf("  bump: %dx%d channels=%d\n", cur->bump->width, cur->bump->height, cur->bump->channels);
		cur = cur->next;
		++i;
	}
}
