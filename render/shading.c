#include "../include/minirt.h"

static t_color	calculate_shadow_color(const t_point *point, \
	const t_light *light, const t_object *objects)
{
	t_ray			shadow_ray;
	t_vec3			light_dir;
	double			light_dist;
	int				i;
	t_color			sum;
	unsigned long	seed;
	t_point			sample_pos;

	if (!ENABLE_SOFT_SHADOWS)
	{
		light_dir = vec_sub(light->center, *point);
		light_dist = vec_len(light_dir);
		if (light_dist <= EPSILON)
			return ((t_color){0.0, 0.0, 0.0});
		light_dir = vec_mult(light_dir, 1.0 / light_dist);
		shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
		shadow_ray.direction = light_dir;
		return (shadow_transmittance_color(objects, &shadow_ray, \
			light_dist - 0.001));
	}
	sum = (t_color){0.0, 0.0, 0.0};
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
			sum = color_add(sum, shadow_transmittance_color(objects, &shadow_ray, \
				light_dist - 0.001));
		}
		i++;
	}
	return (color_scale(sum, 1.0 / (double)SOFT_SHADOW_SAMPLES));
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

	mod_rec = *rec;
	mod_rec.color = get_checker_color(rec);
	view_dir = vec_mult(*ray_dir, -1.0);
	c = calculate_ambient(&scene->ambient_light, &mod_rec.color);
	nowlight = scene->lights;
	while (nowlight)
	{
		{
			t_color	shadow_c;
			t_color	contrib;

			shadow_c = calculate_shadow_color(&mod_rec.point, nowlight, \
				scene->objects);
			if (shadow_c.x > 0.0 || shadow_c.y > 0.0 || shadow_c.z > 0.0)
			{
				light_dir = vec_normalize(\
					vec_sub(nowlight->center, mod_rec.point));
				contrib = color_add(diffuse(nowlight, &mod_rec, &light_dir), \
					specular(nowlight, &mod_rec, &view_dir, &light_dir));
				c = color_add(c, color_mult(contrib, color_scale(shadow_c, 255.0)));
			}
		}
		nowlight = nowlight->next;
	}
	c = clamp255(c);
	return (c);
}
