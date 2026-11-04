#include "../include/mesh.h"

/**
 * Bounding box of the vertices that faces actually use.
 */
static t_aabb	used_bounds(t_objfile *f)
{
	t_aabb	b;
	t_face	*faces;
	t_vec3	*pos;
	size_t	i;

	b = aabb_empty();
	faces = f->faces.data;
	pos = f->pos.data;
	i = 0;
	while (i < f->faces.len)
	{
		aabb_grow(&b, pos[faces[i].v[0]]);
		aabb_grow(&b, pos[faces[i].v[1]]);
		aabb_grow(&b, pos[faces[i].v[2]]);
		i++;
	}
	return (b);
}

/**
 * Triangles in object space: (p - center) * scale, so the model is centered
 * on the origin and its largest side is 1.
 */
static void	fill_tris(t_objfile *f, t_mesh *m, t_vec3 center, double scale)
{
	t_face	*fc;
	t_vec3	*p;
	size_t	i;

	p = f->pos.data;
	i = 0;
	while (i < f->faces.len)
	{
		fc = (t_face *)f->faces.data + i;
		m->tris[i].v0 = vec_mult(vec_sub(p[fc->v[0]], center), scale);
		m->tris[i].e1 = vec_mult(vec_sub(p[fc->v[1]], p[fc->v[0]]), scale);
		m->tris[i].e2 = vec_mult(vec_sub(p[fc->v[2]], p[fc->v[0]]), scale);
		ft_memcpy(m->attrs[i].n, fc->n, sizeof(fc->n));
		ft_memcpy(m->attrs[i].uv, fc->uv, sizeof(fc->uv));
		m->attrs[i].mat = fc->mat;
		i++;
	}
}

/**
 * Hands normals, texture coordinates and materials over to the mesh.
 */
static void	take_arrays(t_objfile *f, t_mesh *m)
{
	m->normals = f->normals.data;
	m->normal_count = (int)f->normals.len;
	m->uvs = f->uvs.data;
	m->uv_count = (int)f->uvs.len;
	m->mats = f->mats.data;
	m->mat_count = (int)f->mats.len;
	ft_bzero(&f->normals, sizeof(t_arr));
	ft_bzero(&f->uvs, sizeof(t_arr));
	ft_bzero(&f->mats, sizeof(t_arr));
}

void	obj_build_mesh(t_objfile *f, t_mesh *m)
{
	t_aabb	b;
	t_vec3	ext;
	double	size;

	b = used_bounds(f);
	ext = vec_sub(b.max, b.min);
	size = fmax(ext.x, fmax(ext.y, ext.z));
	if (!(size > 1e-12) || f->faces.len > 2000000000)
		obj_fatal(f, "model has no size or is too big");
	m->tri_count = (int)f->faces.len;
	m->tris = malloc(sizeof(t_tri) * f->faces.len);
	m->attrs = malloc(sizeof(t_tri_attr) * f->faces.len);
	if (!m->tris || !m->attrs)
		obj_fatal(f, "out of memory");
	fill_tris(f, m, vec_mult(vec_add(b.min, b.max), 0.5), 1.0 / size);
	take_arrays(f, m);
}
