#include "../include/mesh.h"

/*
** .obj files are right-handed (looking down -z, +x is on the right) while
** this renderer is left-handed (looking down -z, -x is on the right). To show
** models the way every other viewer does, x is mirrored on load. Mirroring
** flips the winding of the faces too, which obj_face() undoes.
*/

static void	obj_vertex(t_objfile *f, char *s)
{
	t_vec3	p;

	if (!parse_real(&s, &p.x) || !parse_real(&s, &p.y)
		|| !parse_real(&s, &p.z))
		obj_fatal(f, "invalid vertex");
	p.x = -p.x;
	arr_push(f, &f->pos, &p);
}

static void	obj_texcoord(t_objfile *f, char *s)
{
	t_vec3	t;

	t.y = 0.0;
	t.z = 0.0;
	if (!parse_real(&s, &t.x))
		obj_fatal(f, "invalid texture coordinate");
	s = skip_ws(s);
	if (*s && !parse_real(&s, &t.y))
		obj_fatal(f, "invalid texture coordinate");
	arr_push(f, &f->uvs, &t);
}

static void	obj_normal(t_objfile *f, char *s)
{
	t_vec3	n;

	if (!parse_real(&s, &n.x) || !parse_real(&s, &n.y)
		|| !parse_real(&s, &n.z))
		obj_fatal(f, "invalid normal");
	n.x = -n.x;
	n = vec_normalize(n);
	arr_push(f, &f->normals, &n);
}

/**
 * "s off" / "s 0" turns smoothing off for the following faces (flat
 * shading), any other group number turns it on.
 */
static void	obj_smoothing(t_objfile *f, char *s)
{
	long	group;

	s = skip_ws(s);
	if (ft_strncmp(s, "off", 3) == 0)
		f->cur_smooth = 0;
	else if (parse_index(&s, &group) && group >= 0 && group < 1000000)
		f->cur_smooth = (int)group;
	else
		f->cur_smooth = 1;
}

void	obj_statement(t_objfile *f, char *line)
{
	char	*rest;

	line = skip_ws(line);
	if (*line == '\0' || *line == '#')
		return ;
	if (is_keyword(line, "v", &rest))
		obj_vertex(f, rest);
	else if (is_keyword(line, "vt", &rest))
		obj_texcoord(f, rest);
	else if (is_keyword(line, "vn", &rest))
		obj_normal(f, rest);
	else if (is_keyword(line, "f", &rest))
		obj_face(f, rest);
	else if (is_keyword(line, "usemtl", &rest))
		obj_usemtl(f, rest);
	else if (is_keyword(line, "mtllib", &rest))
		obj_mtllib(f, rest);
	else if (is_keyword(line, "s", &rest))
		obj_smoothing(f, rest);
	else if (!is_ignored_statement(line))
		f->unknown++;
}
