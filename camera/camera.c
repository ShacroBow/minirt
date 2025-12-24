#include "../include/minirt.h"

void	move_camera(t_camera *cam, t_vec3 direction, double speed)
{
	t_vec3	move;

	move = vec_mult(direction, speed);
	cam->center = vec_add(cam->center, move);
}

t_vec3	find_right(t_vec3 forward)
{
	if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		return (vec_normalize(vec_cross(forward, (t_vec3){1, 0, 0})));
	else
		return (vec_normalize(vec_cross(forward, (t_vec3){0, 1, 0})));
}

void	rotate_camera(t_camera *cam, double pitch, double yaw)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	double	c;
	double	s;

	forward = cam->normal;
	right = find_right(forward);
	up = vec_cross(right, forward);
	if (fabs(pitch) > EPSILON)
	{
		c = cos(pitch);
		s = sin(pitch);
		forward = vec_add(vec_mult(forward, c), vec_mult(up, s));
		forward = vec_normalize(forward);
	}
	if (fabs(yaw) > EPSILON)
	{
		c = cos(yaw);
		s = sin(yaw);
		forward = vec_add(vec_mult(forward, c), vec_mult(right, s));
		forward = vec_normalize(forward);
	}
	cam->normal = forward;
}

void	move_element_camera(t_program *prog, int keycode)
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
		move_camera(&prog->scene->camera, forward_xz, prog->move_speed);
	else if (keycode == KEY_S)
		move_camera(&prog->scene->camera, forward_xz, -prog->move_speed);
	else if (keycode == KEY_D)
		move_camera(&prog->scene->camera, right, -prog->move_speed);
	else if (keycode == KEY_A)
		move_camera(&prog->scene->camera, right, prog->move_speed);
	else if (keycode == KEY_PG_UP)
		move_camera(&prog->scene->camera, (t_vec3){0, 1, 0}, \
		prog->move_speed * 2.0);
	else if (keycode == KEY_PG_DOWN)
		move_camera(&prog->scene->camera, (t_vec3){0, -1, 0}, \
		prog->move_speed * 2.0);
}
