#include "../include/minirt.h"

t_vec3	vec_refract(t_vec3 uv, t_vec3 n, double etai_over_etat)
{
	double	cos_theta;
	t_vec3	r_out_perp;
	t_vec3	r_out_parallel;

	cos_theta = fmin(vec_dot(vec_mult(uv, -1.0), n), 1.0);
	r_out_perp = vec_mult(vec_add(uv, vec_mult(n, cos_theta)), etai_over_etat);
	r_out_parallel = vec_mult(n, -sqrt(fabs(1.0 - vec_len_squared(r_out_perp))));
	return (vec_add(r_out_perp, r_out_parallel));
}

double	schlick(double cosine, double ref_idx)
{
	double	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}

double	vec_len_squared(t_vec3 v)
{
	return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
