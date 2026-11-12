#include "../include/minirt.h"

void	validate_reflectivity(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " reflectivity invalid.");
	if (!is_valid_float(str) || !check_range(ft_atof(str), 0.0, 1.0))
		erorrf(scene, NULL, obj_name, " reflectivity invalid.");
}

void	validate_checker_color(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " checker color invalid.");
	if (!check_color_fmt(str))
		erorrf(scene, NULL, obj_name, " checker color invalid.");
}

void	validate_transparency(char *str, t_scene *scene, const char *obj_name)
{
	if (!is_valid_float(str) || !check_range(ft_atof(str), 0.0, 1.0))
		erorrf(scene, NULL, obj_name, " transparency invalid.");
}

void	validate_ior(char *str, t_scene *scene, const char *obj_name)
{
	if (!is_valid_float(str) || !check_range(ft_atof(str), 1.0, 10.0))
		erorrf(scene, NULL, obj_name, " refractive index invalid.");
}

/**
 * Validates the value of one key=value argument whose key is known valid.
 */
void	validate_arg_value(char *arg, t_scene *scene, const char *obj_name)
{
	if (ft_strncmp(arg, "r=", 2) == 0)
		validate_reflectivity(arg + 2, scene, obj_name);
	else if (ft_strncmp(arg, "ch=", 3) == 0)
		validate_checker_color(arg + 3, scene, obj_name);
	else if (ft_strncmp(arg, "tx=", 3) == 0)
		validate_texture(arg + 3, scene, obj_name);
	else if (ft_strncmp(arg, "bump=", 5) == 0)
		validate_bumpmap(arg + 5, scene, obj_name);
	else if (ft_strncmp(arg, "u=", 2) == 0)
		validate_u_scale(arg + 2, scene, obj_name);
	else if (ft_strncmp(arg, "v=", 2) == 0)
		validate_v_scale(arg + 2, scene, obj_name);
	else if (ft_strncmp(arg, "tr=", 3) == 0)
		validate_transparency(arg + 3, scene, obj_name);
	else if (ft_strncmp(arg, "ior=", 4) == 0)
		validate_ior(arg + 4, scene, obj_name);
	else if (ft_strncmp(arg, "rot=", 4) == 0)
		validate_rotation(arg + 4, scene, obj_name);
	else if (ft_strncmp(arg, "em=", 3) == 0)
		validate_emission(arg + 3, scene, obj_name);
}
