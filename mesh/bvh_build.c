#include "../include/mesh.h"

static void	tri_bounds(t_bvh_build *b, int i)
{
	const t_tri	*t;

	t = &b->m->tris[i];
	b->tb[i] = aabb_empty();
	aabb_grow(&b->tb[i], t->v0);
	aabb_grow(&b->tb[i], vec_add(t->v0, t->e1));
	aabb_grow(&b->tb[i], vec_add(t->v0, t->e2));
	b->tc[i] = vec_mult(vec_add(b->tb[i].min, b->tb[i].max), 0.5);
	b->idx[i] = i;
}

static void	bvh_prepare(t_bvh_build *b)
{
	int	i;

	b->tb = malloc(sizeof(t_aabb) * b->m->tri_count);
	b->tc = malloc(sizeof(t_vec3) * b->m->tri_count);
	b->idx = malloc(sizeof(int) * b->m->tri_count);
	b->m->nodes = malloc(sizeof(t_bvh_node) * b->m->tri_count * 2);
	if (!b->tb || !b->tc || !b->idx || !b->m->nodes)
		bvh_fail(b, "out of memory");
	i = 0;
	while (i < b->m->tri_count)
	{
		tri_bounds(b, i);
		i++;
	}
}

/**
 * Splits a node in two while that makes rays cheaper. Small nodes, deep
 * nodes and nodes that can't be split stay leaves.
 */
static void	subdivide(t_bvh_build *b, int ni, int depth)
{
	t_bvh_node	*n;
	t_bvh_split	s;
	int			left;
	int			child;

	n = &b->m->nodes[ni];
	if (n->count <= 2 || depth >= BVH_MAX_DEPTH || !find_split(b, n, &s))
		return ;
	if (n->count <= BVH_LEAF_SIZE && s.cost >= aabb_area(&n->box) * n->count)
		return ;
	left = bvh_partition(b, n, &s);
	if (left == 0 || left == n->count)
		return ;
	child = b->m->node_count;
	b->m->node_count += 2;
	node_init(b, child, n->first, left);
	node_init(b, child + 1, n->first + left, n->count - left);
	n->first = child;
	n->count = 0;
	subdivide(b, child, depth + 1);
	subdivide(b, child + 1, depth + 1);
}

/**
 * Stores the triangles in BVH order, so every leaf is one contiguous run.
 */
static void	bvh_reorder(t_bvh_build *b)
{
	t_tri		*tris;
	t_tri_attr	*attrs;
	int			i;

	tris = malloc(sizeof(t_tri) * b->m->tri_count);
	attrs = malloc(sizeof(t_tri_attr) * b->m->tri_count);
	if (!tris || !attrs)
	{
		free(tris);
		free(attrs);
		bvh_fail(b, "out of memory");
	}
	i = 0;
	while (i < b->m->tri_count)
	{
		tris[i] = b->m->tris[b->idx[i]];
		attrs[i] = b->m->attrs[b->idx[i]];
		i++;
	}
	free(b->m->tris);
	free(b->m->attrs);
	b->m->tris = tris;
	b->m->attrs = attrs;
}

void	bvh_build(t_objfile *f, t_mesh *m)
{
	t_bvh_build	b;

	b.f = f;
	b.m = m;
	b.tb = NULL;
	b.tc = NULL;
	b.idx = NULL;
	bvh_prepare(&b);
	m->node_count = 1;
	node_init(&b, 0, 0, m->tri_count);
	subdivide(&b, 0, 0);
	bvh_reorder(&b);
	free(b.tb);
	free(b.tc);
	free(b.idx);
}
