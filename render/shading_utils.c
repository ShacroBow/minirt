#include "../include/minirt.h"

t_color	calculate_ambient(const t_ambient_light *ambient, \
					const t_color *material_color)
{
	return (color_scale(color_mult(*material_color, ambient->color), \
				ambient->ratio));
}

t_color	diffuse(const t_light *light,
		const t_hit_record *rec, const t_vec3 *light_dir)
{
	double		diffuse_intensity;
	double		tint;
	t_color		mat_mult;
	t_color		mixed;

	diffuse_intensity = fmax(0.0, vec_dot(rec->normal, *light_dir));
	tint = 0.8;
	mat_mult = color_mult(rec->color, color_add(light->color,
				(t_vec3){1, 1, 1}));
	mixed = color_add(mat_mult, color_scale(light->color, tint));
	return (color_scale(mixed, light->ratio * diffuse_intensity));
}

t_color	specular(const t_light *light,
		const t_hit_record *rec, const t_vec3 *view_dir,
		const t_vec3 *light_dir)
{
	t_vec3	reflect_dir;
	double	dot;
	double	spec;
	t_color	white;

	reflect_dir = vec_mult(rec->normal, 2.0 * vec_dot(*light_dir, rec->normal));
	reflect_dir = vec_sub(reflect_dir, *light_dir);
	dot = fmax(0.0, vec_dot(reflect_dir, *view_dir));
	spec = pow(dot, SHININESS);
	white = (t_color){255.0, 255.0, 255.0};
	return (color_scale(color_mult(white, light->color), \
				light->ratio * spec * 0.5));
}
