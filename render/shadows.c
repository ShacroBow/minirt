#include "../include/minirt.h"

static double	clamp01_local(double x)
{
	if (x < 0.0)
		return (0.0);
	if (x > 1.0)
		return (1.0);
	return (x);
}

static double	color01_from_255(double c)
{
	return (clamp01_local(c / 255.0));
}

static t_color	mul_color01(t_color a, t_color b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return (a);
}

static t_color	shadow_filter_from_hit(const t_hit_record *rec)
{
	double	alpha;
	double	tint;
	t_color	base;

	alpha = clamp01_local(rec->transparency);
	if (!ENABLE_TINTED_SHADOWS)
		return ((t_color){alpha, alpha, alpha});
	tint = clamp01_local(SHADOW_TINT_INTENSITY);
	base.x = color01_from_255(rec->color.x);
	base.y = color01_from_255(rec->color.y);
	base.z = color01_from_255(rec->color.z);
	base.x = 1.0 - tint + tint * base.x;
	base.y = 1.0 - tint + tint * base.y;
	base.z = 1.0 - tint + tint * base.z;
	return ((t_color){(1.0 - alpha) + alpha * base.x,
		(1.0 - alpha) + alpha * base.y,
		(1.0 - alpha) + alpha * base.z});
}

static t_color	beer_lambert(t_color trans, double dist)
{
	double	k;
	double	atten;

	if (!ENABLE_BEER_LAMBERT)
		return (trans);
	k = SHADOW_ABSORPTION_STRENGTH;
	atten = exp(-k * dist);
	return (color_scale(trans, atten));
}

t_color	shadow_transmittance_color(const t_object *world, const t_ray *ray, \
	double t_max)
{
	t_hit_record	rec;
	double		remaining;
	t_ray		step_ray;
	t_color		trans;

	trans = (t_color){1.0, 1.0, 1.0};
	remaining = t_max;
	step_ray = *ray;
	while (remaining > EPSILON)
	{
		if (!hit(world, &step_ray, remaining, &rec))
			return (beer_lambert(trans, t_max - remaining));
		trans = mul_color01(trans, shadow_filter_from_hit(&rec));
		if (trans.x < 0.001 && trans.y < 0.001 && trans.z < 0.001)
			return ((t_color){0.0, 0.0, 0.0});
		step_ray.origin = vec_add(rec.point, vec_mult(step_ray.direction, 0.001));
		remaining -= rec.t;
	}
	return (beer_lambert(trans, t_max));
}

double	shadow_transmittance(const t_object *world, const t_ray *ray, \
	double t_max)
{
	t_color	c;

	c = shadow_transmittance_color(world, ray, t_max);
	return ((c.x + c.y + c.z) * 0.33333333333333);
}
