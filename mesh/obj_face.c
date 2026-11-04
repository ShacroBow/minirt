#include "../include/mesh.h"

/**
 * Reads one face corner: "v", "v/vt", "v//vn" or "v/vt/vn".
 * Missing vt / vn are returned as 0.
 */
static bool	parse_ref(char **s, long ref[3])
{
	char	*p;

	p = *s;
	ref[1] = 0;
	ref[2] = 0;
	if (!parse_index(&p, &ref[0]))
		return (false);
	if (*p == '/')
	{
		p++;
		if (*p != '/' && *p && !is_blank(*p) && !parse_index(&p, &ref[1]))
			return (false);
		if (*p == '/')
		{
			p++;
			if (*p && !is_blank(*p) && !parse_index(&p, &ref[2]))
				return (false);
		}
	}
	if (*p && !is_blank(*p))
		return (false);
	*s = p;
	return (true);
}

/**
 * .obj indices start at 1, negative ones count back from the last element
 * read so far (-1 = the latest). Returns a 0-based index.
 */
static int	resolve_index(t_objfile *f, long idx, size_t count)
{
	if (idx > 0 && (size_t)idx <= count)
		return ((int)(idx - 1));
	if (idx < 0 && (size_t)(-idx) <= count)
		return ((int)((long)count + idx));
	obj_fatal(f, "face index out of range");
}

static void	read_corner(t_objfile *f, char **s, int out[3])
{
	long	ref[3];

	if (!parse_ref(s, ref))
		obj_fatal(f, "invalid face");
	out[0] = resolve_index(f, ref[0], f->pos.len);
	out[1] = -1;
	out[2] = -1;
	if (ref[1] != 0)
		out[1] = resolve_index(f, ref[1], f->uvs.len);
	if (ref[2] != 0)
		out[2] = resolve_index(f, ref[2], f->normals.len);
}

/**
 * A face with n corners becomes n - 2 triangles (fan around the first
 * corner). Corners are emitted in reverse order because x was mirrored.
 */
void	obj_face(t_objfile *f, char *s)
{
	int	first[3];
	int	prev[3];
	int	cur[3];
	int	count;

	count = 0;
	s = skip_ws(s);
	while (*s)
	{
		read_corner(f, &s, cur);
		if (count == 0)
			ft_memcpy(first, cur, sizeof(first));
		else if (count >= 2)
			obj_emit_triangle(f, first, cur, prev);
		ft_memcpy(prev, cur, sizeof(prev));
		count++;
		s = skip_ws(s);
	}
	if (count < 3)
		obj_fatal(f, "face needs at least 3 vertices");
}
