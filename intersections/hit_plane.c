/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:11 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:12 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

static bool	plane_t(const t_plane *pl, const t_ray *ray, double *t)
{
	double	denom;
	t_vec3	diff;

	denom = vec_dot(ray->direction, pl->normal);
	if (fabs(denom) < EPSILON)
		return (false);
	diff = vec_sub(pl->point, ray->origin);
	*t = vec_dot(diff, pl->normal) / denom;
	return (*t > EPSILON);
}

static void	set_plane_hit(const t_plane *pl, const t_ray *ray, double t,
			t_hit_record *rec)
{
	rec->t = t;
	rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
	rec->normal = pl->normal;
	if (vec_dot(ray->direction, rec->normal) > 0)
	{
		rec->normal = vec_mult(rec->normal, -1);
		rec->front_face = false;
	}
	else
		rec->front_face = true;
}

bool	hit_plane(const t_plane *pl, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	double	t;

	if (!plane_t(pl, ray, &t) || t >= t_max)
		return (false);
	if (rec)
		set_plane_hit(pl, ray, t, rec);
	return (true);
}
