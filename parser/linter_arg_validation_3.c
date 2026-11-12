#include "../include/minirt.h"

/**
 * em=<strength>: the object glows in its own color, from 0 (off) to 10.
 */
void	validate_emission(char *str, t_scene *scene, const char *obj_name)
{
	if (!is_valid_float(str) || !check_range(ft_atof(str), 0.0, 10.0))
		erorrf(scene, NULL, obj_name, " glow (em=) invalid.");
}
