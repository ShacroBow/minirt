#include "../minirt.h"

static t_color	calculate_ambient(const t_ambient_light *ambient,
		const t_color *material_color)
{
	return (color_scale(color_mult(*material_color, ambient->color),
			ambient->ratio));
}

static t_color	calculate_diffuse(const t_light *light,
		const t_hit_record *rec)
{
	t_vec3	light_dir;
	double	diffuse_intensity;

	light_dir = vec_normalize(vec_sub(light->center, rec->point));
	diffuse_intensity = fmax(0.0, vec_dot(rec->normal, light_dir));
	return (color_scale(color_mult(rec->color, light->color),
			light->ratio * diffuse_intensity));
}

static bool	is_point_in_shadow(const t_point *point, const t_light *light,
	const t_object *objects)
{
	t_ray			shadow_ray;
	t_hit_record	temp_rec;
	t_vec3			light_dir;
	double			light_distance;

	light_dir = vec_sub(light->center, *point);
	light_distance = vec_length(light_dir);
	light_dir = vec_normalize(light_dir);
	shadow_ray.origin = vec_add(*point, vec_mult(light_dir, 0.001));
	shadow_ray.direction = light_dir;
	if (hit(objects, &shadow_ray, light_distance - 0.001, &temp_rec))
		return (true);
	return (false);
}

t_color	phong_shading(const t_hit_record *rec, const t_scene *scene)
{
	t_color		final_color;
	t_color		light_contribution;
	t_light		*current_light;

	final_color = calculate_ambient(&scene->ambient_light, &rec->color);
	current_light = scene->lights;
	while (current_light)
	{
		if (!is_point_in_shadow(&rec->point, current_light, scene->objects))
		{
			light_contribution = calculate_diffuse(current_light, rec);
			final_color = color_add(final_color, light_contribution);
		}
		current_light = current_light->next;
	}
	final_color.x = fmin(255.0, final_color.x);
	final_color.y = fmin(255.0, final_color.y);
	final_color.z = fmin(255.0, final_color.z);
	return (final_color);
}
