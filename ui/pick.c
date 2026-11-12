#include "../include/minirt.h"

/**
 * Pixel position of a world point. False if it is behind the camera or
 * outside the window. Needs an up to date setup_camera().
 */
bool	project_point(const t_camera *cam, t_point p, int *sx, int *sy)
{
	t_vec3	d;
	t_vec3	q;
	double	depth;

	d = vec_sub(p, cam->center);
	depth = vec_dot(d, cam->normal);
	if (depth <= EPSILON)
		return (false);
	q = vec_sub(vec_add(cam->center, vec_mult(d, 1.0 / depth)),
			cam->lower_left_corner);
	*sx = (int)(vec_dot(q, cam->horizontal)
			/ vec_lensqrt(cam->horizontal) * WIDTH);
	*sy = (int)(vec_dot(q, cam->vertical)
			/ vec_lensqrt(cam->vertical) * HEIGHT);
	return (*sx >= 0 && *sx < WIDTH && *sy >= 0 && *sy < HEIGHT);
}

/**
 * Lights have no surface to click on: the closest light marker within
 * PICK_RADIUS pixels of the click, if any.
 */
t_object	*pick_light(t_program *prog, int x, int y)
{
	t_object	*cur;
	t_object	*best;
	int			p[2];
	int			best_d;
	int			d;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	best = NULL;
	best_d = PICK_RADIUS * PICK_RADIUS;
	cur = prog->scene->objects;
	while (cur)
	{
		if (cur->type == LIGHT && project_point(&prog->scene->camera,
				object_center(cur), &p[0], &p[1]))
		{
			d = (p[0] - x) * (p[0] - x) + (p[1] - y) * (p[1] - y);
			if (d <= best_d)
				best = cur;
			if (d <= best_d)
				best_d = d;
		}
		cur = cur->next;
	}
	return (best);
}

/**
 * The object visible at pixel (x, y), NULL for the background.
 */
t_object	*pick_object(t_program *prog, int x, int y)
{
	t_ray			ray;
	t_hit_record	rec;

	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	ray = create_ray(&prog->scene->camera, ((double)x + 0.5) / WIDTH,
			((double)y + 0.5) / HEIGHT);
	if (!hit(prog->scene->objects, &ray, INFINITY, &rec))
		return (NULL);
	return (rec.obj);
}

t_vec3	object_center(const t_object *obj)
{
	const t_cone	*co;

	if (obj->type == SPHERE)
		return (((t_sphere *)obj->shape_data)->center);
	if (obj->type == PLANE)
		return (((t_plane *)obj->shape_data)->point);
	if (obj->type == CYLINDER)
		return (((t_cylinder *)obj->shape_data)->center);
	if (obj->type == LIGHT)
		return (((t_light *)obj->shape_data)->center);
	if (obj->type == MESH)
		return (mesh_center(obj->shape_data));
	co = obj->shape_data;
	return (vec_mult(vec_add(co->center, co->apex), 0.5));
}

/**
 * 1 for the first light of the scene file, 2 for the second...
 */
int	light_number(const t_scene *scene, const t_object *obj)
{
	const t_light	*cur;
	int				n;

	n = 1;
	cur = scene->lights;
	while (cur && cur != obj->shape_data)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}
