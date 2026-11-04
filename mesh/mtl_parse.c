#include "../include/mesh.h"

static int	mtl_new(t_objfile *f, char *s)
{
	t_material	m;
	char		*name;

	name = next_token(&s);
	if (!name)
		name = "";
	ft_bzero(&m, sizeof(m));
	m.name = ft_strdup(name);
	if (!m.name)
		obj_fatal(f, "out of memory");
	m.kd = (t_color){255.0, 255.0, 255.0};
	m.dissolve = 1.0;
	arr_push(f, &f->mats, &m);
	return ((int)f->mats.len - 1);
}

/**
 * "Kd r g b" with values 0..1 ("Kd r" means gray). Other forms such as
 * "Kd spectral file" are ignored.
 */
static void	mtl_kd(t_material *m, char *s)
{
	double	rgb[3];

	if (!parse_real(&s, &rgb[0]))
		return ;
	if (!parse_real(&s, &rgb[1]) || !parse_real(&s, &rgb[2]))
	{
		rgb[1] = rgb[0];
		rgb[2] = rgb[0];
	}
	m->kd.x = clamp(rgb[0], 0.0, 1.0) * 255.0;
	m->kd.y = clamp(rgb[1], 0.0, 1.0) * 255.0;
	m->kd.z = clamp(rgb[2], 0.0, 1.0) * 255.0;
}

/**
 * One number after an optional flag ("d -halo 0.5"). invert stores 1 - v,
 * for "Tr" which is the opposite of "d".
 */
static void	mtl_real(char *s, double *out, bool invert)
{
	double	v;

	s = skip_ws(s);
	if (*s == '-' && ft_isalpha(s[1]))
		next_token(&s);
	if (!parse_real(&s, &v))
		return ;
	if (invert)
		v = 1.0 - v;
	*out = v;
}

/**
 * "map_Kd [options] file": the file name is the last token. Only the binary
 * .ppm format of the texture loader is supported.
 */
static void	mtl_map_kd(t_objfile *f, const char *mtl, t_material *m, char *s)
{
	char	*tok;
	char	*name;
	char	*path;

	name = NULL;
	tok = next_token(&s);
	while (tok)
	{
		name = tok;
		tok = next_token(&s);
	}
	if (!name || m->map_kd)
		return ;
	path = path_in_dir(mtl, name);
	if (!path)
		obj_fatal(f, "out of memory");
	if (has_extension(path, ".ppm"))
		m->map_kd = load_ppm(path);
	if (!m->map_kd)
		obj_warn(path, "texture skipped, needs a binary (P6) .ppm file", NULL);
	free(path);
}

void	mtl_statement(t_objfile *f, const char *path, int *cur, char *line)
{
	char		*rest;
	t_material	*m;

	line = skip_ws(line);
	if (is_keyword(line, "newmtl", &rest))
	{
		*cur = mtl_new(f, rest);
		return ;
	}
	if (*cur < 0)
		return ;
	m = (t_material *)f->mats.data + *cur;
	if (is_keyword(line, "Kd", &rest))
		mtl_kd(m, rest);
	else if (is_keyword(line, "d", &rest))
		mtl_real(rest, &m->dissolve, false);
	else if (is_keyword(line, "Tr", &rest))
		mtl_real(rest, &m->dissolve, true);
	else if (is_keyword(line, "Ni", &rest))
		mtl_real(rest, &m->ior, false);
	else if (is_keyword(line, "map_Kd", &rest))
		mtl_map_kd(f, path, m, rest);
}
