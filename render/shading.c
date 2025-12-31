#include "../include/minirt.h"

static bool	is_point_in_shadow(const t_point *point, const t_light *light, \
					const t_object *objects)
{
	t_ray	shadow_ray;
	t_vec3	light_dir;
	double	light_distance;

	light_dir = vec_sub(light->center, *point);
	light_distance = vec_len(light_dir);
	if (light_distance <= EPSILON)
		return (false);
	light_dir = vec_mult(light_dir, 1.0 / light_distance);
	shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
	shadow_ray.direction = light_dir;
	if (hit_any(objects, &shadow_ray, light_distance - 0.001))
		return (true);
	return (false);
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
		if (!is_point_in_shadow(&mod_rec.point, nowlight, scene->objects))
		{
			light_dir = vec_normalize(\
				vec_sub(nowlight->center, mod_rec.point));
			c = color_add(c, diffuse(nowlight, &mod_rec, &light_dir));
			c = color_add(c, \
				specular(nowlight, &mod_rec, &view_dir, &light_dir));
		}
		nowlight = nowlight->next;
	}
	c = clamp255(c);
	return (c);
}
