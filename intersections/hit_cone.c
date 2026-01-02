#include "../include/minirt.h"

// static bool	quadratic(double a, double b, double c, double *t)
// {
// 	double	discriminant;
// 	double	sqrt_disc;
// 	double	t0;
// 	double	t1;

// 	if (fabs(a) < EPSILON)
// 		return (false);
// 	discriminant = b * b - 4.0 * a * c;
// 	if (discriminant < 0.0)
// 		return (false);
// 	sqrt_disc = sqrt(discriminant);
// 	t0 = (-b - sqrt_disc) / (2.0 * a);
// 	t1 = (-b + sqrt_disc) / (2.0 * a);
// 	if (t0 > EPSILON && (t0 < t1 || t1 <= EPSILON))
// 	{
// 		*t = t0;
// 		return (true);
// 	}
// 	if (t1 > EPSILON)
// 	{
// 		*t = t1;
// 		return (true);
// 	}
// 	return (false);
// }

// CONE EQUATION: ( (P−A) ⋅ V )^2 = ∣ P − A ∣ 2 (cosθ)^2
//	P: any point on the surface of the cone
//	A: apex (top point) of the cone
//	V: direction of the vector from center to apex
//	0: half angle of the cone
//	h: height of the cone

// RAY EQUATION: R(t) = O + tD
//	R(t): position of any point on the rays vector
//	O: origin of the vector
//	t: how far the point is from origin
//	D: direction of the ray

// 

// static t_vec3	cone_coeffs(const t_cone *co, const t_ray *ray)
// {
// 	t_vec3	oc;
// 	t_vec3	dir;
// 	double	k;
// 	t_vec3	coeffs;

// 	oc = vec_sub(ray->origin, co->apex);
// 	dir = co->normal;
// 	k = (co->diameter * 0.5) / co->height;
// 	k = k * k;
// 	coeffs.x = vec_dot(ray->direction, ray->direction)
// 		- (1.0 + k) * vec_dot(ray->direction, dir)
// 		* vec_dot(ray->direction, dir);
// 	coeffs.y = 2.0 * (vec_dot(ray->direction, oc)
// 			- (1.0 + k) * vec_dot(ray->direction, dir)
// 			* vec_dot(oc, dir));
// 	coeffs.z = vec_dot(oc, oc)
// 		- (1.0 + k) * vec_dot(oc, dir) * vec_dot(oc, dir);
// 	return (coeffs);
// }

static void	set_cone_hit(const t_cone *co, const t_ray *ray, double t, \
						t_hit_record *rec)
{
	t_vec3	hit_point;
	t_vec3	q;
	t_vec3	normal_vec;
	double	k;

	rec->t = t;
	hit_point = vec_add(ray->origin, vec_mult(ray->direction, t));
	rec->point = hit_point;
	q = vec_sub(hit_point, co->apex);
	k = (co->diameter * 0.5 / co->height);
	k = k * k;
	normal_vec = vec_sub(q, vec_mult(co->normal, \
		(1.0 + k) * vec_dot(q, co->normal)));
	rec->normal = vec_normalize(normal_vec);
	if (vec_dot(ray->direction, rec->normal) > 0)
	{
		rec->normal = vec_mult(rec->normal, -1);
		rec->front_face = false;
	}
	else
		rec->front_face = true;
}

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

static bool	check_caps(const t_cone *co, const t_ray *ray, double *t,
			t_hit_record *rec)
{
	t_cap	bottom;
	bool	hit;

	bottom.center = co->center;
	bottom.normal = co->normal;
	bottom.r2 = (co->diameter * co->diameter) / 4.0;
	bottom.invert = 0;
	hit = false;
	if (cap_hit(&bottom, ray, t, rec))
		hit = true;
	return (hit);
}

static bool	try_cone_body_hit(const t_cone *co, const t_ray *ray,
			double t_max, t_hit_record *rec)
{
	t_vec3	coeffs;
	t_vec3	hit_point;
	double	t;
	double	dist;

	coeffs = cone_coeffs(co, ray);
	if (!quadratic(coeffs.x, coeffs.y, coeffs.z, &t) || t >= t_max)
		return (false);
	hit_point = vec_add(ray->origin, vec_mult(ray->direction, t));
	dist = vec_dot(vec_sub(hit_point, co->apex), co->normal);
	if (dist < -co->height || dist > 0)
		return (false);
	if (rec)
		set_cone_hit(co, ray, t, rec);
	return (true);
}

bool	hit_cone(const t_cone *co, const t_ray *ray, double t_max, \
		t_hit_record *rec)
{
	double	t;

	if (try_cone_body_hit(co, ray, t_max, rec))
		return (true);
	t = t_max;
	if (!check_caps(co, ray, &t, rec))
		return (false);
	if (rec)
	{
		rec->t = t;
		rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
		rec->front_face = (vec_dot(ray->direction, rec->normal) < 0);
	}
	return (true);
}
