#include "../include/minirt.h"

/**
 * How much light from target reaches point (per color channel, 0..1).
 */
static t_color	shadow_towards(const t_point *point, t_point target,
	const t_object *objects)
{
	t_ray	shadow_ray;
	t_vec3	light_dir;
	double	light_dist;

	light_dir = vec_sub(target, *point);
	light_dist = vec_len(light_dir);
	if (light_dist <= EPSILON)
		return ((t_color){0.0, 0.0, 0.0});
	light_dir = vec_mult(light_dir, 1.0 / light_dist);
	shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
	shadow_ray.direction = light_dir;
	return (shadow_transmittance_color(objects, &shadow_ray,
			light_dist - 0.001));
}

/**
 * Different random light samples for every surface point, so soft shadow
 * edges turn into fine noise instead of stacked copies of a hard shadow.
 */
static unsigned long	point_seed(const t_point *p)
{
	return (((unsigned long)(long)fmod(p->x * 4096.0, 1e15) * 73856093UL)
		^ ((unsigned long)(long)fmod(p->y * 4096.0, 1e15) * 19349663UL)
		^ ((unsigned long)(long)fmod(p->z * 4096.0, 1e15) * 83492791UL));
}

static t_color	calculate_shadow_color(const t_point *point,
	const t_light *light, const t_object *objects)
{
	t_color			sum;
	unsigned long	seed;
	t_point			sample;
	int				i;

	if (!ENABLE_SOFT_SHADOWS)
		return (shadow_towards(point, light->center, objects));
	sum = (t_color){0.0, 0.0, 0.0};
	seed = point_seed(point);
	i = 0;
	while (i < SOFT_SHADOW_SAMPLES)
	{
		sample.x = light->center.x + (fast_rand(&seed) - 0.5) * LIGHT_RADIUS;
		sample.y = light->center.y + (fast_rand(&seed) - 0.5) * LIGHT_RADIUS;
		sample.z = light->center.z + (fast_rand(&seed) - 0.5) * LIGHT_RADIUS;
		sum = color_add(sum, shadow_towards(point, sample, objects));
		i++;
	}
	return (color_scale(sum, 1.0 / (double)SOFT_SHADOW_SAMPLES));
}

static t_color	light_contribution(const t_hit_record *rec,
	const t_light *light, const t_scene *scene, const t_vec3 *view_dir)
{
	t_color	shadow_c;
	t_color	contrib;
	t_vec3	light_dir;

	shadow_c = calculate_shadow_color(&rec->point, light, scene->objects);
	if (shadow_c.x <= 0.0 && shadow_c.y <= 0.0 && shadow_c.z <= 0.0)
		return ((t_color){0.0, 0.0, 0.0});
	light_dir = vec_normalize(vec_sub(light->center, rec->point));
	contrib = color_add(diffuse(light, rec, &light_dir),
			specular(light, rec, view_dir, &light_dir));
	return (color_mult(contrib, color_scale(shadow_c, 255.0)));
}

t_color	phong_shading(const t_hit_record *rec, const t_scene *scene,
	const t_vec3 *ray_dir)
{
	t_color			c;
	t_light			*nowlight;
	t_vec3			view_dir;
	t_hit_record	mod_rec;

	mod_rec = *rec;
	mod_rec.color = get_checker_color(rec);
	view_dir = vec_mult(*ray_dir, -1.0);
	c = calculate_ambient(&scene->ambient_light, &mod_rec.color);
	nowlight = scene->lights;
	while (nowlight)
	{
		c = color_add(c,
				light_contribution(&mod_rec, nowlight, scene, &view_dir));
		nowlight = nowlight->next;
	}
	c.x = fmin(c.x, 255.0);
	c.y = fmin(c.y, 255.0);
	c.z = fmin(c.z, 255.0);
	return (c);
}
