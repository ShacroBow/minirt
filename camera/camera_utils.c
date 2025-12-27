#include "../include/minirt.h"

static void	debug_helper(int *is_cam, t_object **object)
{
	if (*is_cam)
		printf("selected cam\n");
	else if (*object && (*object)->type == SPHERE)
		printf("selected SPHERE\n");
	else if (*object && (*object)->type == CYLINDER)
		printf("selected CYLINDER\n");
	else if (*object && (*object)->type == PLANE)
		printf("selected PLANE\n");
	else if (*object && (*object)->type == CONE)
		printf("selected CONE\n");
}

void	select_element(int *is_cam, t_object **object, int keycode, \
						t_program *prog)
{
	if (keycode == KEY_0)
		*is_cam = 1;
	else if (keycode == KEY_3)
	{
		*is_cam = 0;
		if (!*object)
			*object = prog->scene->objects;
		else
			*object = (*object)->next;
		if (!*object)
			*object = prog->scene->objects;
		if (!*object)
			*is_cam = 1;
	}
	if (DEBUG)
	{
		debug_helper(is_cam, object);
	}
}

void	move_object(t_object *object, t_vec3 direction, double speed)
{
	t_vec3	move;

	move = vec_mult(direction, speed);
	if (object->type == SPHERE)
		((t_sphere *)object->shape_data)->center = \
			vec_add(((t_sphere *)object->shape_data)->center, move);
	else if (object->type == CYLINDER)
		((t_cylinder *)object->shape_data)->center = \
			vec_add(((t_cylinder *)object->shape_data)->center, move);
	else if (object->type == PLANE)
		((t_plane *)object->shape_data)->point = \
			vec_add(((t_plane *)object->shape_data)->point, move);
	else if (object->type == CONE)
	{
		((t_cone *)object->shape_data)->center = \
			vec_add(((t_cone *)object->shape_data)->center, move);
		((t_cone *)object->shape_data)->apex = \
			vec_add(((t_cone *)object->shape_data)->apex, move);
	}
}

void	move_element_object(t_program *prog, int keycode, t_object *object)
{
	t_vec3	forward;
	t_vec3	forward_xz;
	t_vec3	right;

	forward = prog->scene->camera.normal;
	if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		right = vec_normalize(vec_cross(forward, (t_vec3){1, 0, 0}));
	else
		right = vec_normalize(vec_cross(forward, (t_vec3){0, 1, 0}));
	forward_xz = vec_normalize((t_vec3){forward.x, 0, forward.z});
	if (keycode == KEY_W)
		move_object(object, forward_xz, prog->move_speed);
	if (keycode == KEY_S)
		move_object(object, forward_xz, -prog->move_speed);
	if (keycode == KEY_D)
		move_object(object, right, -prog->move_speed);
	if (keycode == KEY_A)
		move_object(object, right, prog->move_speed);
	if (keycode == KEY_PG_UP)
		move_object(object, (t_vec3){0, 1, 0}, prog->move_speed * 2.0);
	if (keycode == KEY_PG_DOWN)
		move_object(object, (t_vec3){0, -1, 0}, prog->move_speed * 2.0);
}

void	move_element(int is_cam, t_object *object, t_program *prog, int keycode)
{
	if (is_cam)
		move_element_camera(prog, keycode);
	else
		move_element_object(prog, keycode, object);
}
