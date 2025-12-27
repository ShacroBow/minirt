#include "../include/minirt.h"

void	camera_keystuffing(int keycode, t_program *prog, \
		t_object **object, int *is_cam)
{
	(void)object;
	(void)is_cam;
	if (keycode == KEY_O)
		prog->aa_enabled = !prog->aa_enabled;
	else if (keycode == KEY_1)
	{
		prog->pixel_step += PIXEL_STEP_INC;
		if (prog->pixel_step > PIXEL_STEP_MAX)
			prog->pixel_step = PIXEL_STEP_MAX;
	}
	else if (keycode == KEY_2)
	{
		prog->pixel_step -= PIXEL_STEP_INC;
		if (prog->pixel_step < PIXEL_STEP_MIN)
			prog->pixel_step = PIXEL_STEP_MIN;
	}
}
