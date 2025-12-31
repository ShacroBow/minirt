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
		return (sample_texture(rec->obj->texture, uv[0] - floor(uv[0]),
				uv[1] - floor(uv[1])));
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
