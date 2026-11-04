#include "../include/mesh.h"

/**
 * Slab test. t_near is where the ray enters the box.
 */
bool	box_hit(const t_aabb *b, const t_mray *r, double *t_near)
{
	double	t0;
	double	t1;
	double	lo;
	double	hi;

	t0 = (b->min.x - r->o.x) * r->inv.x;
	t1 = (b->max.x - r->o.x) * r->inv.x;
	lo = fmin(t0, t1);
	hi = fmax(t0, t1);
	t0 = (b->min.y - r->o.y) * r->inv.y;
	t1 = (b->max.y - r->o.y) * r->inv.y;
	lo = fmax(lo, fmin(t0, t1));
	hi = fmin(hi, fmax(t0, t1));
	t0 = (b->min.z - r->o.z) * r->inv.z;
	t1 = (b->max.z - r->o.z) * r->inv.z;
	lo = fmax(lo, fmin(t0, t1));
	hi = fmin(hi, fmax(t0, t1));
	*t_near = lo;
	return (hi >= lo && hi > EPSILON && lo < r->t_max);
}

static void	walk_push(t_walk *w, int node, double t)
{
	if (w->sp >= BVH_STACK_SIZE)
		return ;
	w->node[w->sp] = node;
	w->t[w->sp] = t;
	w->sp++;
}

/**
 * Pushes the children the ray hits, the nearer one last so it is visited
 * first: once a close triangle is found most far boxes can be skipped.
 */
static void	push_children(const t_mesh *m, const t_mray *r, int first,
				t_walk *w)
{
	double	tl;
	double	tr;
	bool	hl;
	bool	hr;

	hl = box_hit(&m->nodes[first].box, r, &tl);
	hr = box_hit(&m->nodes[first + 1].box, r, &tr);
	if (hl && hr && tl < tr)
	{
		walk_push(w, first + 1, tr);
		walk_push(w, first, tl);
	}
	else if (hl && hr)
	{
		walk_push(w, first, tl);
		walk_push(w, first + 1, tr);
	}
	else if (hl)
		walk_push(w, first, tl);
	else if (hr)
		walk_push(w, first + 1, tr);
}

static void	leaf_hit(const t_mesh *m, const t_bvh_node *n, t_mray *r)
{
	int	i;

	i = n->first;
	while (i < n->first + n->count)
	{
		if (tri_hit(&m->tris[i], r, i) && r->any)
			return ;
		i++;
	}
}

/**
 * Closest hit along r (any hit if r->any). Returns true if something was hit.
 */
bool	bvh_walk(const t_mesh *m, t_mray *r)
{
	t_walk				w;
	const t_bvh_node	*n;
	double				t;

	w.sp = 0;
	if (!box_hit(&m->nodes[0].box, r, &t))
		return (false);
	walk_push(&w, 0, t);
	while (w.sp > 0)
	{
		w.sp--;
		if (w.t[w.sp] < r->t_max)
		{
			n = &m->nodes[w.node[w.sp]];
			if (n->count > 0)
				leaf_hit(m, n, r);
			else
				push_children(m, r, n->first, &w);
			if (r->any && r->tri >= 0)
				return (true);
		}
	}
	return (r->tri >= 0);
}
