#include "../include/mesh.h"

static void	set_corner(t_face *face, int k, const int *corner)
{
	face->v[k] = corner[0];
	face->uv[k] = corner[1];
	face->n[k] = corner[2];
}

/**
 * Normals and texture coordinates are only used if all three corners have
 * one; a half specified triangle is treated as having none.
 */
static void	drop_partial(int attr[3])
{
	if (attr[0] < 0 || attr[1] < 0 || attr[2] < 0)
	{
		attr[0] = -1;
		attr[1] = -1;
		attr[2] = -1;
	}
}

/**
 * Stores the triangle a, b, c (each {vertex, uv, normal}). Triangles using
 * the same vertex twice have no area and are skipped.
 */
void	obj_emit_triangle(t_objfile *f, const int *a, const int *b,
			const int *c)
{
	t_face	face;

	if (a[0] == b[0] || b[0] == c[0] || a[0] == c[0])
		return ;
	set_corner(&face, 0, a);
	set_corner(&face, 1, b);
	set_corner(&face, 2, c);
	drop_partial(face.uv);
	drop_partial(face.n);
	face.mat = f->cur_mat;
	face.smooth = f->cur_smooth;
	arr_push(f, &f->faces, &face);
}
