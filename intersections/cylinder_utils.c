#include "../minirt.h"

static bool solve_cy_quadratic(double a, double b, double c, double *t)
{
    double discriminant;
    double sqrt_discriminant;
    double t1;
    double t2;

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (false);
    sqrt_discriminant = sqrt(discriminant);
    t1 = (-b - sqrt_discriminant) / (2 * a);
    t2 = (-b + sqrt_discriminant) / (2 * a);
    if (t1 > 0.000001 && t1 < *t)
        *t = t1;
    else if (t2 > 0.000001 && t2 < *t)
        *t = t2;
    else
        return (false);
    return (true);
}

void calculate_cylinder_coefficients(const t_cylinder *cy,
                                     const t_ray *ray, t_quad_coef *coef)
{
    t_vec3 oc;
    double dn;
    double on;

    oc = vec_sub(ray->origin, cy->center);
    dn = vec_dot(ray->direction, cy->normal);
    on = vec_dot(oc, cy->normal);
    coef->a = vec_length_squared(ray->direction) - dn * dn;
    coef->b = 2 * (vec_dot(ray->direction, oc) - dn * on);
    coef->c = vec_length_squared(oc) - on * on - (cy->diameter * cy->diameter) / 4.0;
}

bool set_cylinder_hit_data(const t_cylinder *cy, const t_ray *ray,
                           double t, t_hit_record *rec)
{
    t_vec3 oc;
    double dn;
    double on;
    double y;

    oc = vec_sub(ray->origin, cy->center);
    dn = vec_dot(ray->direction, cy->normal);
    on = vec_dot(oc, cy->normal);
    y = on + t * dn;
    if (fabs(y) > cy->height / 2.0)
        return (false);
    rec->t = t;
    rec->point = vec_add(ray->origin, vec_mult(ray->direction, t));
    rec->normal = vec_normalize(vec_sub(vec_sub(rec->point, cy->center),
                                        vec_mult(cy->normal, y)));
    if (vec_dot(ray->direction, rec->normal) > 0)
        rec->normal = vec_mult(rec->normal, -1);
    rec->front_face = true;
    return (true);
}

bool side_hit(const t_cylinder *cy, const t_ray *ray, double t_max,
              t_hit_record *rec)
{
    t_quad_coef coef;
    double t;

    calculate_cylinder_coefficients(cy, ray, &coef);
    t = t_max;
    if (!solve_cy_quadratic(coef.a, coef.b, coef.c, &t) || t >= t_max)
        return (false);
    return (set_cylinder_hit_data(cy, ray, t, rec));
}
