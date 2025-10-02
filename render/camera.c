#include "../minirt.h"

static void calculate_viewport(t_camera *cam, double aspect_ratio)
{
	double theta;
	double h;

	theta = cam->fov * M_PI / 180.0;
	h = tan(theta / 2.0);
	cam->viewport_height = 2.0 * h;
	cam->viewport_width = aspect_ratio * cam->viewport_height;
}

static void calculate_basis_vectors(t_camera *cam, t_vec3 *w, t_vec3 *u,
									t_vec3 *v)
{
	*w = vec_normalize(vec_mult(cam->normal, -1));
	if (fabs(w->x) < EPSILON && fabs(w->z) < EPSILON)
		*u = vec_normalize(vec_cross(*w, (t_vec3){1, 0, 0}));
	else
		*u = vec_normalize(vec_cross(*w, (t_vec3){0, 1, 0}));
	*v = vec_cross(*w, *u);
}

static void setup_camera_vectors(t_camera *cam, t_vec3 w, t_vec3 u, t_vec3 v)
{
	cam->horizontal = vec_mult(u, cam->viewport_width);
	cam->vertical = vec_mult(v, cam->viewport_height);
	cam->lower_left_corner = vec_sub(cam->center,
									 vec_div(cam->horizontal, 2.0));
	cam->lower_left_corner = vec_sub(cam->lower_left_corner,
									 vec_div(cam->vertical, 2.0));
	cam->lower_left_corner = vec_sub(cam->lower_left_corner, w);
	cam->up_dir = v; /* cache unit up */
}

void setup_camera(t_camera *cam, double aspect_ratio)
{
	t_vec3 w;
	t_vec3 u;
	t_vec3 v;

	calculate_viewport(cam, aspect_ratio);
	calculate_basis_vectors(cam, &w, &u, &v);
	setup_camera_vectors(cam, w, u, v);
}

t_ray create_ray(const t_camera *cam, double u, double v)
{
	t_ray r;
	t_vec3 horizontal_part;
	t_vec3 vertical_part;

	r.origin = cam->center;
	horizontal_part = vec_mult(cam->horizontal, u);
	vertical_part = vec_mult(cam->vertical, v);
	r.direction = vec_sub(vec_add(cam->lower_left_corner,
								  vec_add(horizontal_part, vertical_part)),
						  r.origin);
	r.direction = vec_normalize(r.direction);
	return (r);
}