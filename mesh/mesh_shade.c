#include "../include/mesh.h"

/**
 * Barycentric interpolation of three per-corner values.
 */
static t_vec3	lerp3(t_vec3 a, t_vec3 b, t_vec3 c, const t_mray *r)
{
	return (vec_add(vec_mult(a, 1.0 - r->b1 - r->b2),
			vec_add(vec_mult(b, r->b1), vec_mult(c, r->b2))));
}

/**
 * Interpolated vertex normal in world space, or the flat face normal if the
 * triangle has no vertex normals.
 */
static t_vec3	shading_normal(const t_mesh *m, const t_tri_attr *a,
					const t_mray *r, t_vec3 flat)
{
	t_vec3	n;

	if (a->n[0] < 0 || a->n[1] < 0 || a->n[2] < 0)
		return (flat);
	n = lerp3(m->normals[a->n[0]], m->normals[a->n[1]],
			m->normals[a->n[2]], r);
	if (vec_lensqrt(n) < 1e-12)
		return (flat);
	return (vec_normalize(mat_mul(m->rot, n)));
}

static void	set_uv(const t_mesh *m, const t_tri_attr *a, const t_mray *r,
				t_hit_record *rec)
{
	t_vec3	uv;

	rec->has_tex_uv = (a->uv[0] >= 0 && a->uv[1] >= 0 && a->uv[2] >= 0);
	if (!rec->has_tex_uv)
		return ;
	uv = lerp3(m->uvs[a->uv[0]], m->uvs[a->uv[1]], m->uvs[a->uv[2]], r);
	rec->tex_u = uv.x;
	rec->tex_v = uv.y;
}

void	mesh_fill_hit(const t_mesh *m, const t_ray *ray, const t_mray *r,
			t_hit_record *rec)
{
	const t_tri			*t;
	const t_tri_attr	*a;
	t_vec3				flat;
	t_vec3				n;

	t = &m->tris[r->tri];
	a = &m->attrs[r->tri];
	rec->t = r->t_max;
	rec->point = vec_add(ray->origin, vec_mult(ray->direction, r->t_max));
	flat = vec_normalize(mat_mul(m->rot, vec_cross(t->e1, t->e2)));
	n = shading_normal(m, a, r, flat);
	if (vec_dot(flat, n) < 0.0)
		flat = vec_mult(flat, -1.0);
	rec->front_face = (vec_dot(ray->direction, flat) < 0.0);
	if (!rec->front_face)
		n = vec_mult(n, -1.0);
	rec->normal = n;
	set_uv(m, a, r, rec);
	rec->mat_id = a->mat;
}

/**
 * Applies the .mtl material of the hit triangle: its color (times its
 * texture, if any) tinted by the object color from the scene, and its
 * transparency. Called after the generic object fields were set.
 */
void	mesh_apply_material(const t_object *obj, t_hit_record *rec)
{
	const t_mesh		*m;
	const t_material	*mat;
	t_color				c;

	m = (const t_mesh *)obj->shape_data;
	if (rec->mat_id < 0 || rec->mat_id >= m->mat_count)
		return ;
	mat = &m->mats[rec->mat_id];
	c = mat->kd;
	if (mat->map_kd && rec->has_tex_uv)
		c = color_mult(c, sample_texture(mat->map_kd,
					rec->tex_u - floor(rec->tex_u),
					rec->tex_v - floor(rec->tex_v)));
	rec->color = color_mult(c, obj->color);
	if (mat->dissolve < 1.0)
	{
		rec->transparency = fmax(rec->transparency,
				1.0 - fmax(mat->dissolve, 0.0));
		if (mat->ior >= 1.0)
			rec->refractive_index = mat->ior;
	}
}
