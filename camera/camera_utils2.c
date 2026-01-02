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

void	scale_object_hor(t_object *object, double amount)
{
	t_cone	*co;
	double	prev_height;

	if (object->type == SPHERE)
		((t_sphere *)object->shape_data)->diameter
			= clamp(((t_sphere *)object->shape_data)->diameter + amount * 5,
				0, 1000000);
	else if (object->type == CYLINDER)
		((t_cylinder *)object->shape_data)->height
			= clamp(((t_cylinder *)object->shape_data)->height + amount * 5,
				0, 1000000);
	else if (object->type == CONE)
	{
		co = ((t_cone *)object->shape_data);
		prev_height = co->height;
		co->height = clamp(prev_height + amount * 5, 0, 1000000);
		co->center = vec_sub(co->center, vec_mult(co->normal,
					(co->height - prev_height)));
	}
	else if (object->type == LIGHT)
		((t_light *)object->shape_data)->ratio
			= clamp(((t_light *)object->shape_data)->ratio + amount / 2.0,
				0, 1);
}

void	scale_object_ver(t_object *object, double amount)
{
	if (object->type == CYLINDER)
		((t_cylinder *)object->shape_data)->diameter
			= clamp(((t_cylinder *)object->shape_data)->diameter + amount * 5,
				0, 1000000);
	else if (object->type == CONE)
		((t_cone *)object->shape_data)->diameter
			= clamp(((t_cone *)object->shape_data)->diameter + amount * 5,
				0, 1000000);
}
