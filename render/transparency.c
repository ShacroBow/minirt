#include "../include/minirt.h"

static void	setup_refraction(t_hit_record *rec, double *etai, t_vec3 *normal)
{
	*normal = rec->normal;
	if (rec->front_face)
		*etai = 1.0 / rec->refractive_index;
	else
		*etai = rec->refractive_index;
}

static t_color	calc_reflection(t_hit_record *rec, t_vec3 unit_dir, \
								t_vec3 normal, t_program *prog)
{
	t_ray	scattered;

	scattered.origin = vec_add(rec->point, vec_mult(normal, 0.001));
	scattered.direction = vec_reflect(unit_dir, normal);
	return (trace_ray_recursive(&scattered, prog, (int)rec->reflect_depth - 1));
}

static t_color	calc_refraction(t_hit_record *rec, t_vec3 unit_dir, \
								t_vec3 normal, t_program *prog)
{
	t_ray	scattered;
	double	etai;

	if (rec->front_face)
		etai = 1.0 / rec->refractive_index;
	else
		etai = rec->refractive_index;
	scattered.direction = vec_refract(unit_dir, normal, etai);
	scattered.origin = vec_add(rec->point, \
						vec_mult(scattered.direction, 0.01));
	return (trace_ray_recursive(&scattered, prog, (int)rec->reflect_depth - 1));
}

/**
 * Fresnel: how much of the light is reflected (1 = total reflection).
 */
static double	reflect_chance(t_hit_record *rec, t_vec3 unit_dir,
					t_vec3 *normal)
{
	double	etai;
	double	cos_theta;

	setup_refraction(rec, &etai, normal);
	cos_theta = fmin(vec_dot(vec_mult(unit_dir, -1.0), *normal), 1.0);
	if (etai * sqrt(1.0 - cos_theta * cos_theta) > 1.0)
		return (1.0);
	return (schlick(cos_theta, etai));
}

/**
 * Glass: mix of the reflected and refracted ray. Progressive passes follow
 * only one of them, picked at random with the Fresnel weight, so a ray
 * never splits; the average over the passes gives the same mix.
 */
t_color	handle_transparency(const t_ray *ray, t_hit_record *rec,
	t_program *prog, int depth)
{
	t_vec3	unit_dir;
	t_vec3	normal;
	double	p;

	rec->reflect_depth = depth;
	unit_dir = vec_normalize(ray->direction);
	p = reflect_chance(rec, unit_dir, &normal);
	if (prog->scene->progressive)
		p = (point_random(&rec->point, prog->scene->sample) < p);
	if (p > 0.999)
		return (calc_reflection(rec, unit_dir, normal, prog));
	if (p < 0.001)
		return (calc_refraction(rec, unit_dir, normal, prog));
	return (color_add(
			color_scale(calc_reflection(rec, unit_dir, normal, prog), p),
			color_scale(calc_refraction(rec, unit_dir, normal, prog), 1 - p)));
}
