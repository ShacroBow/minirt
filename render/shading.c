#include "../minirt.h"

static t_color	calculate_diffuse(const t_hit_record *rec, const t_light *light,
		t_vec3 light_dir)
{
	double	diffuse_strength;

	diffuse_strength = fmax(0.0, vec_dot(rec->normal, light_dir));
	return (color_scale(light->color, diffuse_strength * light->ratio));
}

static t_color	calculate_specular(const t_hit_record *rec, const t_light *light,
		t_vec3 light_dir, const t_scene *scene)
{
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	specular_strength;
	t_color	specular_color;

	view_dir = vec_normalize(vec_sub(scene->camera.center, rec->point));
	reflect_dir = vec_reflect(vec_mult(light_dir, -1), rec->normal);
	specular_strength = pow(fmax(0.0, vec_dot(view_dir, reflect_dir)),
			SHININESS);
	specular_color = color_scale(light->color, specular_strength
			* light->ratio);
	return (specular_color);
}

t_color	phong_shading(const t_hit_record *rec, const t_scene *scene)
{
	t_color	final_color;
	t_light	*current_light;
	t_vec3	light_dir;
	t_ray	shadow_ray;
	double	light_dist;

	final_color = color_mult(rec->color,
			color_scale(scene->ambient_light.color,
				scene->ambient_light.ratio));
	current_light = scene->lights;
	while (current_light)
	{
		light_dir = vec_sub(current_light->center, rec->point);
		light_dist = vec_length(light_dir);
		light_dir = vec_normalize(light_dir);
		shadow_ray.origin = vec_add(rec->point, vec_mult(rec->normal, EPSILON));
		shadow_ray.direction = light_dir;
		if (!hit(scene->objects, &shadow_ray, light_dist, &((t_hit_record){0})))
		{
			final_color = color_add(final_color,
					color_mult(rec->color, calculate_diffuse(rec, current_light,
						light_dir)));
			final_color = color_add(final_color,
					calculate_specular(rec, current_light, light_dir, scene));
		}
		current_light = current_light->next;
	}
	return (final_color);
}