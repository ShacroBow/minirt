#include "../include/minirt.h"

static void	compute_basis_for_rec(const t_hit_record *rec, t_vec3 *u_axis,
		t_vec3 *v_axis)
{
    t_vec3 normal = rec->normal;
    if (rec->type == PLANE)
    {
        t_plane *pl = (t_plane *)rec->obj->shape_data;
        normal = pl->normal;
    }
    else if (rec->type == CYLINDER)
    {
        t_cylinder *cy = (t_cylinder *)rec->obj->shape_data;
        normal = cy->normal;
    }
    else if (rec->type == CONE)
    {
        t_cone *co = (t_cone *)rec->obj->shape_data;
        normal = co->center_dir;
    }
    /* choose arbitrary axis not parallel to normal */
    t_vec3 arbitrary = (fabs(normal.y) > 0.9) ? (t_vec3){1,0,0} : (t_vec3){0,1,0};
    *u_axis = vec_normalize(vec_cross(arbitrary, normal));
    *v_axis = vec_cross(normal, *u_axis);
}

void	apply_bump(t_hit_record *rec, const t_ray *ray)
{
    double u, v;
    t_texture *tex;

    if (!rec || !rec->obj)
        return;
    if (!rec->obj->has_bump || !rec->obj->bump || !rec->obj->bump_enabled)
        return;
    tex = rec->obj->bump;
    if (!tex || tex->width <= 0 || tex->height <= 0)
        return;
    if (!compute_uv(rec, &u, &v))
        return;
    t_vec3 u_axis, v_axis;
    compute_basis_for_rec(rec, &u_axis, &v_axis);
    /* wrap uv into [0,1) to match texture tiling used elsewhere */
    /* Interpret bump texture as a tangent-space normal map (RGB -> normal) */
    double u_wrapped = (u * rec->obj->uv_scale_u);
    double v_wrapped = (v * rec->obj->uv_scale_v);
    u_wrapped = u_wrapped - floor(u_wrapped);
    v_wrapped = v_wrapped - floor(v_wrapped);

    t_color c = sample_texture(tex, u_wrapped, v_wrapped);
    /* convert from [0,255] to [-1,1] */
    double nx = (c.x / 255.0) * 2.0 - 1.0;
    double ny = (c.y / 255.0) * 2.0 - 1.0;
    double nz = (c.z / 255.0) * 2.0 - 1.0;
    t_vec3 n_tangent = { nx, ny, nz };
    n_tangent = vec_normalize(n_tangent);

    /* build world-space normal from tangent-space normal */
    t_vec3 obj_normal = rec->normal;
    if (rec->type == PLANE)
    {
        t_plane *pl = (t_plane *)rec->obj->shape_data;
        obj_normal = pl->normal;
    }
    else if (rec->type == CYLINDER)
    {
        t_cylinder *cy = (t_cylinder *)rec->obj->shape_data;
        obj_normal = cy->normal;
    }
    else if (rec->type == CONE)
    {
        t_cone *co = (t_cone *)rec->obj->shape_data;
        obj_normal = co->center_dir;
    }

    t_vec3 world_n = {0,0,0};
    world_n = vec_add(world_n, vec_mult(u_axis, n_tangent.x));
    world_n = vec_add(world_n, vec_mult(v_axis, n_tangent.y));
    world_n = vec_add(world_n, vec_mult(obj_normal, n_tangent.z));
    rec->normal = vec_normalize(world_n);
    /* recompute front face */
    if (vec_dot(ray->direction, rec->normal) > 0)
    {
        rec->normal = vec_mult(rec->normal, -1);
        rec->front_face = false;
    }
    else
        rec->front_face = true;
}
