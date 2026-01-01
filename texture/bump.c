#include "../include/minirt.h"

static t_vec3	get_geometric_normal(const t_hit_record *rec)
{
	if (rec->type == PLANE)
		return (((t_plane *)rec->obj->shape_data)->normal);
	if (rec->type == CYLINDER)
		return (((t_cylinder *)rec->obj->shape_data)->normal);
	if (rec->type == CONE)
		return (((t_cone *)rec->obj->shape_data)->center_dir);
	return (rec->normal);
}

static void	compute_basis_for_rec(const t_hit_record *rec, t_vec3 *u_axis, \
		t_vec3 *v_axis)
{
	t_vec3	arbitrary;
	t_vec3	normal;

	normal = get_geometric_normal(rec);
	if (fabs(normal.y) > 0.9)
		arbitrary = (t_vec3){1, 0, 0};
	else
		arbitrary = (t_vec3){0, 1, 0};
	*u_axis = vec_normalize(vec_cross(arbitrary, normal));
	*v_axis = vec_cross(normal, *u_axis);
}

static t_vec3	sample_bump_normal(t_texture *tex, double u, double v, \
	t_object *obj)
{
	t_color	c;
	double	uv_w[2];
	t_vec3	n;

	uv_w[0] = (u * obj->scale_u);
	uv_w[1] = (v * obj->scale_v);
	uv_w[0] = uv_w[0] - floor(uv_w[0]);
	uv_w[1] = uv_w[1] - floor(uv_w[1]);
	c = sample_texture(tex, uv_w[0], uv_w[1]);
	n.x = (c.x / 255.0) * 2.0 - 1.0;
	n.y = (c.y / 255.0) * 2.0 - 1.0;
	n.z = (c.z / 255.0) * 2.0 - 1.0;
	return (vec_normalize(n));
}

static void	perturb_normal(t_hit_record *rec, t_vec3 n_tangent, \
	t_vec3 u_axis, t_vec3 v_axis)
{
	t_vec3	world_n;
	t_vec3	obj_n;

	obj_n = get_geometric_normal(rec);
	world_n = (t_vec3){0, 0, 0};
	world_n = vec_add(world_n, vec_mult(u_axis, n_tangent.x));
	world_n = vec_add(world_n, vec_mult(v_axis, n_tangent.y));
	world_n = vec_add(world_n, vec_mult(obj_n, n_tangent.z));
	rec->normal = vec_normalize(world_n);
}

void	apply_bump(t_hit_record *rec, const t_ray *ray)
{
	double	uv[2];
	t_vec3	axes[2];
	t_vec3	n_tangent;

	if (!rec || !rec->obj || !rec->obj->has_bump || !rec->obj->bump \
		|| !rec->obj->bump_enabled)
		return ;
	if (rec->obj->bump->width <= 0 || rec->obj->bump->height <= 0 \
		|| !compute_uv(rec, &uv[0], &uv[1]))
		return ;
	compute_basis_for_rec(rec, &axes[0], &axes[1]);
	n_tangent = sample_bump_normal(rec->obj->bump, uv[0], uv[1], rec->obj);
	perturb_normal(rec, n_tangent, axes[0], axes[1]);
	if (vec_dot(ray->direction, rec->normal) > 0)
	{
		rec->normal = vec_mult(rec->normal, -1);
		rec->front_face = false;
	}
	else
		rec->front_face = true;
}
