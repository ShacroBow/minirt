#include "../include/mesh.h"

/**
 * "Ks r g b" / "Ke r g b" (one value means gray), each clamped to 0..max.
 */
static bool	mtl_rgb(char *s, t_color *out, double max)
{
	double	rgb[3];

	if (!parse_real(&s, &rgb[0]))
		return (false);
	if (!parse_real(&s, &rgb[1]) || !parse_real(&s, &rgb[2]))
	{
		rgb[1] = rgb[0];
		rgb[2] = rgb[0];
	}
	out->x = clamp(rgb[0], 0.0, max);
	out->y = clamp(rgb[1], 0.0, max);
	out->z = clamp(rgb[2], 0.0, max);
	return (true);
}

/**
 * Reflection and glow, what a .mtl material adds to its plain color:
 *   illum 3 to 7  mirror reflection; 5 and 7 reflect more at grazing
 *                 angles (Fresnel), like car paint or water
 *   Ks r g b      how much is reflected (the average of r, g and b)
 *   Ke r g b      glow color, shown whatever the lighting (may exceed 1)
 * Other illum values (0 to 2: plain shading) keep Ks out of it, so common
 * exported files do not turn into mirrors.
 */
void	mtl_shine(t_material *m, char *line)
{
	char	*rest;
	t_color	c;
	double	illum;

	if (is_keyword(line, "Ks", &rest) && mtl_rgb(rest, &c, 1.0))
		m->ks = (c.x + c.y + c.z) / 3.0;
	else if (is_keyword(line, "Ke", &rest) && mtl_rgb(rest, &c, 10.0))
		m->ke = vec_mult(c, 255.0);
	else if (is_keyword(line, "illum", &rest) && parse_real(&rest, &illum))
		m->illum = (int)illum;
}

/**
 * The material's reflection replaces the object's (r=) when it is
 * stronger; its glow is tinted by the object color, like its color.
 */
void	mtl_apply_shine(const t_material *mat, const t_object *obj,
			t_hit_record *rec)
{
	if (mat->illum >= 3 && mat->illum <= 7 && mat->ks > rec->reflect)
	{
		rec->reflect = mat->ks;
		rec->fresnel = (mat->illum == 5 || mat->illum == 7);
	}
	rec->emission = color_mult(mat->ke, obj->color);
}
