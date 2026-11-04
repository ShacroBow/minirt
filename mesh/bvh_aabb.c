#include "../include/mesh.h"

t_aabb	aabb_empty(void)
{
	t_aabb	b;

	b.min = (t_vec3){INFINITY, INFINITY, INFINITY};
	b.max = (t_vec3){-INFINITY, -INFINITY, -INFINITY};
	return (b);
}

void	aabb_grow(t_aabb *b, t_vec3 p)
{
	b->min.x = fmin(b->min.x, p.x);
	b->min.y = fmin(b->min.y, p.y);
	b->min.z = fmin(b->min.z, p.z);
	b->max.x = fmax(b->max.x, p.x);
	b->max.y = fmax(b->max.y, p.y);
	b->max.z = fmax(b->max.z, p.z);
}

void	aabb_merge(t_aabb *b, const t_aabb *o)
{
	if (o->min.x > o->max.x)
		return ;
	aabb_grow(b, o->min);
	aabb_grow(b, o->max);
}

double	aabb_area(const t_aabb *b)
{
	t_vec3	d;

	if (b->min.x > b->max.x)
		return (0.0);
	d = vec_sub(b->max, b->min);
	return (2.0 * (d.x * d.y + d.y * d.z + d.z * d.x));
}

double	vec_axis(t_vec3 v, int axis)
{
	if (axis == 0)
		return (v.x);
	if (axis == 1)
		return (v.y);
	return (v.z);
}
