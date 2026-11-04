#include "../include/minirt.h"

static t_color	mix_colors(t_color c1, t_color c2, double t)
{
	return (color_add(color_scale(c1, t), color_scale(c2, 1.0 - t)));
}

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

t_color	handle_transparency(const t_ray *ray, t_hit_record *rec, \
							t_program *prog, int depth)
{
	t_vec3	unit_dir;
	double	etai;
	t_vec3	normal;
	double	cos_theta;
	double	reflect_prob;

	rec->reflect_depth = depth;
	unit_dir = vec_normalize(ray->direction);
	setup_refraction(rec, &etai, &normal);
	cos_theta = fmin(vec_dot(vec_mult(unit_dir, -1.0), normal), 1.0);
	if (etai * sqrt(1.0 - cos_theta * cos_theta) > 1.0)
		reflect_prob = 1.0;
	else
		reflect_prob = schlick(cos_theta, etai);
	if (reflect_prob > 0.999)
		return (calc_reflection(rec, unit_dir, normal, prog));
	if (reflect_prob < 0.001)
		return (calc_refraction(rec, unit_dir, normal, prog));
	return (mix_colors(calc_reflection(rec, unit_dir, normal, prog), \
			calc_refraction(rec, unit_dir, normal, prog), reflect_prob));
}
