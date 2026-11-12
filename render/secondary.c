#include "../include/minirt.h"

/**
 * Fresnel (Schlick): surfaces like car paint or water reflect little when
 * seen straight on and nearly everything at grazing angles. rec->reflect is
 * the share reflected straight on.
 */
static double	fresnel_amount(const t_ray *ray, const t_hit_record *rec)
{
	double	c;
	double	c5;

	c = 1.0 + vec_dot(vec_normalize(ray->direction), rec->normal);
	c = clamp(c, 0.0, 1.0);
	c5 = c * c * c * c * c;
	return (rec->reflect + (1.0 - rec->reflect) * c5);
}

/**
 * Everything on top of the local (Phong) color: glass, mirror reflection
 * and glow. rec->reflect_depth is how many more bounces are allowed.
 * A Fresnel surface shows its own color less where it reflects more.
 */
t_color	shade_secondary(const t_ray *ray, t_hit_record *rec, t_program *prog,
	t_color local)
{
	int	depth;

	depth = (int)rec->reflect_depth;
	if (depth > 0 && ENABLE_TRANSPARENCY && rec->transparency > 0.0)
		local = color_add(color_add(color_scale(handle_transparency(ray, rec,
							prog, depth), rec->transparency),
					color_scale(local, 1.0 - rec->transparency)),
				get_specular_only(rec, prog->scene, &ray->direction));
	if (depth > 0 && ENABLE_REFLECTIONS && rec->reflect > 0.0)
	{
		if (rec->fresnel)
		{
			rec->reflect = fresnel_amount(ray, rec);
			local = color_scale(local, 1.0 - rec->reflect);
		}
		local = color_add(local, handle_reflection(ray, rec, prog, depth));
	}
	return (color_add(local, rec->emission));
}
