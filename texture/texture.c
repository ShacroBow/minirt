#include "../include/minirt.h"

static void	get_checker_indices(const t_hit_record *rec, double u, double v,
		int *indices)
{
	if (rec->type == PLANE)
	{
		indices[0] = floor(u * 0.5);
		indices[1] = floor(v * 0.5);
	}
	else if (rec->type == SPHERE)
	{
		indices[0] = floor(u * 10);
		indices[1] = floor(v * 7);
	}
	else
	{
		indices[0] = floor(u * 10);
		indices[1] = floor(v * 2);
	}
}

t_color	get_checker_color(const t_hit_record *rec)
{
	double	uv[2];
	int		indices[2];

	if (rec->obj && rec->obj->has_texture && rec->obj->texture)
	{
		if (!compute_uv(rec, &uv[0], &uv[1]))
			return (rec->color);
		double u_s = (uv[0] * rec->obj->uv_scale_u) - floor(uv[0] * rec->obj->uv_scale_u);
		double v_s = (uv[1] * rec->obj->uv_scale_v) - floor(uv[1] * rec->obj->uv_scale_v);
		// if (DEBUG)
		// 	printf("[DEBUG] sample_texture: obj_type=%d has_texture=%d bump_enabled=%d u_s=%f v_s=%f\n",
		// 		rec->type, rec->obj->has_texture, rec->obj->bump_enabled, u_s, v_s);
		return (sample_texture(rec->obj->texture, u_s, v_s));
	}
	if (!rec->has_checkerboard)
		return (rec->color);
	if (!compute_uv(rec, &uv[0], &uv[1]))
		return (rec->color);
	get_checker_indices(rec, uv[0], uv[1], indices);
	if ((indices[0] + indices[1]) % 2 != 0)
		return (rec->checker_color);
	return (rec->color);
}

t_color	sample_texture(const t_texture *tex, double u, double v)
{
	int		x;
	int		y;
	size_t	idx;

	if (!tex || !tex->data)
		return ((t_color){0.0, 0.0, 0.0});
	u = clamp01(u);
	v = clamp01(v);
	x = (int)(u * (tex->width - 1));
	y = (int)((1.0 - v) * (tex->height - 1));
	idx = ((size_t)y * tex->width + (size_t)x) * tex->channels;
	return ((t_color){(double)tex->data[idx],
		(double)tex->data[idx + 1], (double)tex->data[idx + 2]});
}

