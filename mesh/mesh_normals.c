#include "../include/mesh.h"

/*
** Normals for faces that come without "vn": every corner gets the average of
** the faces around its vertex that are in the same smoothing group and meet
** this face at less than the crease angle. Curved surfaces become smooth and
** hard edges (a cube's corners) stay sharp.
*/

static bool	needs_normals(const t_face *face)
{
	return (face->n[0] < 0 && face->smooth != 0);
}

static t_vec3	corner_normal(t_smooth *sm, int face, int v)
{
	t_face	*faces;
	t_vec3	sum;
	int		i;
	int		g;

	faces = sm->f->faces.data;
	sum = (t_vec3){0.0, 0.0, 0.0};
	i = sm->off[v];
	while (i < sm->off[v + 1])
	{
		g = sm->adj[i];
		if (faces[g].smooth == faces[face].smooth
			&& vec_dot(sm->un[g], sm->un[face]) >= CREASE_COS)
			sum = vec_add(sum, sm->fn[g]);
		i++;
	}
	return (vec_normalize(sum));
}

/**
 * Corners of a smooth area compute bit-identical normals, so a vertex reuses
 * the normal it stored last instead of storing one per corner.
 */
static int	normal_index(t_smooth *sm, int v, t_vec3 n)
{
	t_vec3	*normals;
	int		last;

	last = sm->cache[v];
	normals = sm->f->normals.data;
	if (last >= 0 && normals[last].x == n.x && normals[last].y == n.y
		&& normals[last].z == n.z)
		return (last);
	arr_push(sm->f, &sm->f->normals, &n);
	sm->cache[v] = (int)sm->f->normals.len - 1;
	return (sm->cache[v]);
}

static void	smooth_face(t_smooth *sm, int face)
{
	t_face	*fc;
	t_vec3	n;
	int		k;

	fc = (t_face *)sm->f->faces.data + face;
	k = 0;
	while (k < 3)
	{
		n = corner_normal(sm, face, fc->v[k]);
		if (vec_lensqrt(n) > 0.5)
			fc->n[k] = normal_index(sm, fc->v[k], n);
		k++;
	}
	if (fc->n[0] < 0 || fc->n[1] < 0 || fc->n[2] < 0)
	{
		fc->n[0] = -1;
		fc->n[1] = -1;
		fc->n[2] = -1;
	}
}

void	obj_smooth_normals(t_objfile *f)
{
	t_smooth	sm;
	t_face		*faces;
	size_t		i;

	faces = f->faces.data;
	i = 0;
	while (i < f->faces.len && !needs_normals(&faces[i]))
		i++;
	if (i == f->faces.len)
		return ;
	smooth_init(&sm, f);
	i = 0;
	while (i < f->faces.len)
	{
		if (needs_normals(&faces[i]))
			smooth_face(&sm, (int)i);
		i++;
	}
	smooth_free(&sm);
}
