#include "../include/mesh.h"

static bool	store_hit(t_mray *r, int index, double t, t_vec3 bary)
{
	if (t <= EPSILON || t >= r->t_max)
		return (false);
	r->t_max = t;
	r->tri = index;
	r->b1 = bary.x;
	r->b2 = bary.y;
	return (true);
}

/**
 * Möller-Trumbore ray / triangle test. On a hit closer than r->t_max it
 * records the distance, the triangle and the barycentric coordinates.
 * Both sides of a triangle can be hit.
 */
bool	tri_hit(const t_tri *t, t_mray *r, int index)
{
	t_vec3	p;
	t_vec3	q;
	double	inv;
	double	u;
	double	v;

	p = vec_cross(r->d, t->e2);
	inv = vec_dot(t->e1, p);
	if (fabs(inv) < 1e-30)
		return (false);
	inv = 1.0 / inv;
	q = vec_sub(r->o, t->v0);
	u = vec_dot(q, p) * inv;
	if (u < 0.0 || u > 1.0)
		return (false);
	q = vec_cross(q, t->e1);
	v = vec_dot(r->d, q) * inv;
	if (v < 0.0 || u + v > 1.0)
		return (false);
	return (store_hit(r, index, vec_dot(t->e2, q) * inv,
			(t_vec3){u, v, 0.0}));
}
