#include "../include/mesh.h"

/**
 * 1 / v without dividing by zero: a huge number keeps the slab test free of
 * NaN for rays parallel to an axis.
 */
static double	safe_inv(double v)
{
	if (fabs(v) < 1e-300)
		return (1e300);
	return (1.0 / v);
}

/**
 * World ray -> object space. The direction is scaled as well (not
 * normalized again), so a distance t means the same point in both spaces.
 */
static void	to_object_ray(const t_mesh *m, const t_ray *ray, t_mray *r)
{
	double	inv_size;

	inv_size = 1.0 / m->size;
	r->o = vec_mult(mat_tmul(m->rot, vec_sub(ray->origin, m->center)),
			inv_size);
	r->d = vec_mult(mat_tmul(m->rot, ray->direction), inv_size);
	r->inv.x = safe_inv(r->d.x);
	r->inv.y = safe_inv(r->d.y);
	r->inv.z = safe_inv(r->d.z);
	r->tri = -1;
	r->b1 = 0.0;
	r->b2 = 0.0;
}

/**
 * Like the other hit_* functions: true if the ray hits the mesh before
 * t_max, and fills rec (if given) for the closest triangle.
 */
bool	hit_mesh(const t_mesh *m, const t_ray *ray, double t_max,
			t_hit_record *rec)
{
	t_mray	r;

	if (!m || m->tri_count <= 0 || !m->nodes)
		return (false);
	to_object_ray(m, ray, &r);
	r.t_max = t_max;
	r.any = (rec == NULL);
	if (!bvh_walk(m, &r))
		return (false);
	if (rec)
		mesh_fill_hit(m, ray, &r, rec);
	return (true);
}

void	mesh_translate(t_mesh *m, t_vec3 move)
{
	m->center = vec_add(m->center, move);
}

t_vec3	mesh_center(const t_mesh *m)
{
	return (m->center);
}
