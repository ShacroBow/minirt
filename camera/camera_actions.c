#include "../include/minirt.h"

void	handle_camera_rotation(t_camera *cam, int keycode)
{
	if (keycode == KEY_LEFT)
		rotate_camera(cam, 0, ROT_SPEED);
	else if (keycode == KEY_RIGHT)
		rotate_camera(cam, 0, -ROT_SPEED);
	else if (keycode == KEY_UP)
		rotate_camera(cam, ROT_SPEED, 0);
	else if (keycode == KEY_DOWN)
		rotate_camera(cam, -ROT_SPEED, 0);
}

void	handle_speed_change(t_program *prog, int keycode)
{
	if (keycode == KEY_PLUS)
		prog->move_speed += SPEED_INCREMENT;
	else if (keycode == KEY_MINUS && prog->move_speed > SPEED_INCREMENT)
		prog->move_speed -= SPEED_INCREMENT;
}

void	handle_bump_action(t_object **object, int *is_cam)
{
	if (!*is_cam && object && *object)
	{
		(*object)->bump_enabled = !(*object)->bump_enabled;
		if ((*object)->bump_enabled)
			printf("Toggled bump for selected object: now ENABLED\n");
		else
			printf("Toggled bump for selected object: now DISABLED\n");
	}
}

void	handle_scaling_action(int keycode, t_object **object, int *is_cam)
{
	if (*is_cam || !object || !*object)
		return ;
	if (keycode == KEY_4)
	{
		(*object)->scale_u *= 1.25;
		printf("scale_u = %f\n", (*object)->scale_u);
	}
	else if (keycode == KEY_5)
	{
		(*object)->scale_u /= 1.25;
		printf("scale_u = %f\n", (*object)->scale_u);
	}
	else if (keycode == KEY_6)
	{
		(*object)->scale_v *= 1.25;
		printf("scale_v = %f\n", (*object)->scale_v);
	}
	else if (keycode == KEY_7)
	{
		(*object)->scale_v /= 1.25;
		printf("scale_v = %f\n", (*object)->scale_v);
	}
}
