/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:08 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:09 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

static bool	cap_hit(const t_cap *cap, const t_ray *ray, double *t,
			t_hit_record *rec)
{
	double	denom;
	double	t_cap;
	t_point	p;
	t_vec3	diff;

	denom = vec_dot(ray->direction, cap->normal);
	if (fabs(denom) <= EPSILON)
		return (false);
	t_cap = vec_dot(vec_sub(cap->center, ray->origin), cap->normal) / denom;
	if (t_cap <= EPSILON || t_cap >= *t)
		return (false);
	p = vec_add(ray->origin, vec_mult(ray->direction, t_cap));
	diff = vec_sub(p, cap->center);
	if (vec_lensqrt(diff) >= cap->r2)
		return (false);
	*t = t_cap;
	if (!rec)
		return (true);
	if (cap->invert)
		rec->normal = vec_mult(cap->normal, -1);
	else
		rec->normal = cap->normal;
	return (true);
}

static bool	check_caps(const t_cylinder *cy, const t_ray *ray, double *t,
			t_hit_record *rec)
{
	t_cap	top;
	t_cap	bottom;
	bool	hit;

	top.center = vec_add(cy->center, vec_mult(cy->normal, cy->height / 2));
	top.normal = cy->normal;
	top.r2 = (cy->diameter * cy->diameter) / 4.0;
	top.invert = 0;
	bottom.center = vec_sub(cy->center, vec_mult(cy->normal, cy->height / 2));
	bottom.normal = cy->normal;
	bottom.r2 = top.r2;
	bottom.invert = 1;
	hit = false;
	if (cap_hit(&top, ray, t, rec))
		hit = true;
	if (cap_hit(&bottom, ray, t, rec))
		hit = true;
	return (hit);
}

// static bool	quadratic(double a, double b, double c, double *t)
// {
// 	double	discriminant;
// 	double	t0;
// 	double	t1;

// 	if (fabs(a) < EPSILON)
// 		return (false);
// 	discriminant = b * b - 4 * a * c;
// 	if (discriminant < 0)
// 		return (false);
// 	t0 = (-b - sqrt(discriminant)) / (2 * a);
// 	t1 = (-b + sqrt(discriminant)) / (2 * a);
// 	if (t0 > EPSILON && (t0 < t1 || t1 <= EPSILON))
// 		*t = t0;
// 	else if (t1 > EPSILON)
// 		*t = t1;
// 	else
// 		return (false);
// 	return (true);
// }

static bool	side_hit(const t_cylinder *cy, const t_ray *ray, double t_max,
			t_hit_record *rec)
{
	t_vec3	center;
	double	axis_dir;
	double	cen_dot;
	double	hit_t;
	double	y;

	center = vec_sub(ray->origin, cy->center);
	axis_dir = vec_dot(ray->direction, cy->normal);
	cen_dot = vec_dot(center, cy->normal);
	hit_t = t_max;
	if (!quadratic(1.0 - axis_dir * axis_dir, 2 * (vec_dot(ray->direction, \
	center) - axis_dir * cen_dot), vec_lensqrt(center) - cen_dot * cen_dot - \
	(cy->diameter * cy->diameter) / 4.0, &hit_t) || hit_t >= t_max)
		return (false);
	y = cen_dot + hit_t * axis_dir;
	if (fabs(y) > cy->height / 2)
		return (false);
	if (!rec)
		return (true);
	rec->point = vec_add(ray->origin, vec_mult(ray->direction, hit_t));
	rec->normal = vec_normalize(vec_sub(vec_sub(rec->point, cy->center), \
								vec_mult(cy->normal, y)));
	if (vec_dot(ray->direction, rec->normal) > 0)
		rec->normal = vec_mult(rec->normal, -1);
	return (rec->t = hit_t, rec->front_face = true, true);
}

bool	hit_cylinder(const t_cylinder *cy, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	double			t;
	bool			side;
	bool			cap;
	t_hit_record	side_rec;

	side = side_hit(cy, ray, t_max, &side_rec);
	t = t_max;
	if (side)
		t = side_rec.t;
	cap = check_caps(cy, ray, &t, rec);
	if (cap && rec)
	{
		rec->t = t;
		rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
		rec->front_face = (vec_dot(ray->direction, rec->normal) < 0);
		return (true);
	}
	if (side && rec)
		*rec = side_rec;
	return (side || cap);
}
