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

t_color	get_specular_only(const t_hit_record *rec, const t_scene *scene, \
						const t_vec3 *ray_dir)
{
	t_color	c;
	t_light	*nowlight;
	t_vec3	view_dir;
	t_vec3	light_dir;

	c = (t_color){0, 0, 0};
	view_dir = vec_mult(*ray_dir, -1.0);
	nowlight = scene->lights;
	while (nowlight)
	{
		if (!is_point_in_shadow(&rec->point, nowlight, scene->objects))
		{
			light_dir = vec_normalize(vec_sub(nowlight->center, rec->point));
			c = color_add(c, specular(nowlight, rec, &view_dir, &light_dir));
		}
		nowlight = nowlight->next;
	}
	return (c);
}
