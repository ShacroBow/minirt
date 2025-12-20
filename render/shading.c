#include "../minirt.h"

static t_color calculate_ambient(const t_ambient_light *ambient,
								 const t_color *material_color)
{
	return (color_scale(color_mult(*material_color, ambient->color),
						ambient->ratio));
}

static t_color calculate_diffuse(const t_light *light,
								 const t_hit_record *rec, const t_vec3 *light_dir)
{
	double diffuse_intensity;

	diffuse_intensity = fmax(0.0, vec_dot(rec->normal, *light_dir));
	return (color_scale(color_mult(rec->color, light->color),
					light->ratio * diffuse_intensity));
}static t_color	calculate_specular(const t_light *light,
		const t_hit_record *rec, const t_vec3 *view_dir, const t_vec3 *light_dir)
{
	t_vec3	reflect_dir;
	double	spec;
	double	dot;
	t_color	white;

	reflect_dir = vec_mult(rec->normal, 2.0 * vec_dot(*light_dir, rec->normal));
	reflect_dir = vec_sub(reflect_dir, *light_dir);
	dot = fmax(0.0, vec_dot(reflect_dir, *view_dir));
	spec = dot * dot;
	spec *= spec;
	spec *= spec;
	spec *= spec;
	spec *= spec;
	white = (t_color){255.0, 255.0, 255.0};
	return (color_scale(color_mult(white, light->color),
			light->ratio * spec * 0.5));
}

static bool is_point_in_shadow(const t_point *point, const t_light *light,
							   const t_object *objects)
{
	t_ray shadow_ray;
	t_vec3 light_dir;
	double light_distance;
	double inv_light_distance;

	light_dir = vec_sub(light->center, *point);
	light_distance = vec_length(light_dir);
	inv_light_distance = 1.0 / light_distance;
	light_dir = vec_mult(light_dir, inv_light_distance);
	shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
	shadow_ray.direction = light_dir;
	if (hit_any(objects, &shadow_ray, light_distance - 0.001))
		return (true);
	return (false);
}

t_color phong_shading(const t_hit_record *rec, const t_scene *scene,
					  const t_vec3 *ray_dir)
{
	t_color final_color;
	t_light *current_light;
	t_vec3 view_dir;
	t_vec3 light_dir;
	double light_distance;
	double inv_light_distance;

	view_dir = vec_normalize(vec_mult(*ray_dir, -1.0));
	final_color = calculate_ambient(&scene->ambient_light, &rec->color);
	current_light = scene->lights;
	while (current_light)
	{
		if (!is_point_in_shadow(&rec->point, current_light, scene->objects))
		{
			light_dir = vec_sub(current_light->center, rec->point);
			light_distance = vec_length(light_dir);
			inv_light_distance = 1.0 / light_distance;
			light_dir = vec_mult(light_dir, inv_light_distance);
			final_color = color_add(final_color, 
				calculate_diffuse(current_light, rec, &light_dir));
			final_color = color_add(final_color, 
				calculate_specular(current_light, rec, &view_dir, &light_dir));
		}
		current_light = current_light->next;
	}
	if (final_color.x > 255.0)
		final_color.x = 255.0;
	if (final_color.y > 255.0)
		final_color.y = 255.0;
	if (final_color.z > 255.0)
		final_color.z = 255.0;
	return (final_color);
}
