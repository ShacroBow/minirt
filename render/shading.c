#include "../include/minirt.h"

static double	calculate_shadow_factor(const t_point *point, \
	const t_light *light, const t_object *objects)
{
	t_ray			shadow_ray;
	t_vec3			light_dir;
	double			light_dist;
	int				i;
	int				unblocked;
	unsigned long	seed;
	t_point			sample_pos;

	if (!ENABLE_SOFT_SHADOWS)
	{
		light_dir = vec_sub(light->center, *point);
		light_dist = vec_len(light_dir);
		if (light_dist <= EPSILON)
			return (0.0);
		light_dir = vec_mult(light_dir, 1.0 / light_dist);
		shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
		shadow_ray.direction = light_dir;
		if (hit_any(objects, &shadow_ray, light_dist - 0.001))
			return (0.0);
		return (1.0);
	}
	unblocked = 0;
	seed = (unsigned long)((point->x * 12345 + point->y * 6789 + point->z * 42));
	i = 0;
	while (i < SOFT_SHADOW_SAMPLES)
	{
		sample_pos.x = light->center.x + (fast_rand(&seed) - 0.5) * LIGHT_RADIUS;
		sample_pos.y = light->center.y + (fast_rand(&seed) - 0.5) * LIGHT_RADIUS;
		sample_pos.z = light->center.z + (fast_rand(&seed) - 0.5) * LIGHT_RADIUS;
		light_dir = vec_sub(sample_pos, *point);
		light_dist = vec_len(light_dir);
		if (light_dist > EPSILON)
		{
			light_dir = vec_mult(light_dir, 1.0 / light_dist);
			shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
			shadow_ray.direction = light_dir;
			if (!hit_any(objects, &shadow_ray, light_dist - 0.001))
				unblocked++;
		}
		i++;
	}
	return ((double)unblocked / (double)SOFT_SHADOW_SAMPLES);
}

static t_color	clamp255(t_color c)
{
	if (c.x > 255.0)
		c.x = 255.0;
	if (c.y > 255.0)
		c.y = 255.0;
	if (c.z > 255.0)
		c.z = 255.0;
	return (c);
}

t_color	phong_shading(const t_hit_record *rec, const t_scene *scene,
	const t_vec3 *ray_dir)
{
	t_color			c;
	t_light			*nowlight;
	t_vec3			view_dir;
	t_vec3			light_dir;
	t_hit_record	mod_rec;
	double			shadow_factor;

	mod_rec = *rec;
	mod_rec.color = get_checker_color(rec);
	view_dir = vec_mult(*ray_dir, -1.0);
	c = calculate_ambient(&scene->ambient_light, &mod_rec.color);
	nowlight = scene->lights;
	while (nowlight)
	{
		shadow_factor = calculate_shadow_factor(&mod_rec.point, nowlight, \
			scene->objects);
		if (shadow_factor > 0.0)
		{
			light_dir = vec_normalize(\
				vec_sub(nowlight->center, mod_rec.point));
			c = color_add(c, color_scale(color_add(\
				diffuse(nowlight, &mod_rec, &light_dir), \
				specular(nowlight, &mod_rec, &view_dir, &light_dir)), \
				shadow_factor));
		}
		nowlight = nowlight->next;
	}
	c = clamp255(c);
	return (c);
}
