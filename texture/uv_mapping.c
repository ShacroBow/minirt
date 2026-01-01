#include "../include/minirt.h"

static void	get_sphere_uv(const t_hit_record *rec, double *u, double *v)
{
	*u = 0.5 + atan2(rec->normal.z, rec->normal.x) / (2 * M_PI);
	*v = 0.5 - asin(rec->normal.y) / M_PI;
}

static void	get_plane_uv(const t_hit_record *rec, double *u, double *v)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_plane	*pl;

	pl = (t_plane *)rec->obj->shape_data;
	if (fabs(pl->normal.y) > 0.9)
		u_axis = (t_vec3){1, 0, 0};
	else
		u_axis = (t_vec3){0, 1, 0};
	u_axis = vec_normalize(vec_cross(u_axis, pl->normal));
	v_axis = vec_cross(pl->normal, u_axis);
	*u = vec_dot(rec->point, u_axis);
	*v = vec_dot(rec->point, v_axis);
}

static void	get_cylinder_uv(const t_hit_record *rec, double *u, double *v)
{
	t_cylinder	*cy;
	t_vec3		u_axis;
	t_vec3		v_axis;
	t_vec3		p_center;

	cy = (t_cylinder *)rec->obj->shape_data;
	p_center = vec_sub(rec->point, cy->center);
	*v = vec_dot(p_center, cy->normal);
	if (fabs(cy->normal.y) > 0.9)
		u_axis = (t_vec3){1, 0, 0};
	else
		u_axis = (t_vec3){0, 1, 0};
	u_axis = vec_normalize(vec_cross(u_axis, cy->normal));
	v_axis = vec_cross(cy->normal, u_axis);
	if (fabs(vec_dot(rec->normal, cy->normal)) > 0.95)
	{
		*u = 0.5 + vec_dot(p_center, u_axis) / cy->diameter;
		*v = 0.5 + vec_dot(p_center, v_axis) / cy->diameter;
		return ;
	}
	*u = 0.5 + atan2(vec_dot(p_center, v_axis), \
		vec_dot(p_center, u_axis)) / (2 * M_PI);
}

static void	get_cone_uv(const t_hit_record *rec, double *u, double *v)
{
	t_cone	*co;
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	p_center;

	co = (t_cone *)rec->obj->shape_data;
	p_center = vec_sub(rec->point, co->center);
	*v = vec_dot(p_center, co->normal);
	if (fabs(co->normal.y) > 0.9)
		u_axis = (t_vec3){1, 0, 0};
	else
		u_axis = (t_vec3){0, 1, 0};
	u_axis = vec_normalize(vec_cross(u_axis, co->normal));
	v_axis = vec_cross(co->normal, u_axis);
	*u = 0.5 + atan2(vec_dot(p_center, v_axis), \
					vec_dot(p_center, u_axis)) / (2 * M_PI);
}

int	compute_uv(const t_hit_record *rec, double *u, double *v)
{
	if (rec->type == SPHERE)
		get_sphere_uv(rec, u, v);
	else if (rec->type == PLANE)
		get_plane_uv(rec, u, v);
	else if (rec->type == CYLINDER)
		get_cylinder_uv(rec, u, v);
	else if (rec->type == CONE)
		get_cone_uv(rec, u, v);
	else
		return (0);
	return (1);
}
