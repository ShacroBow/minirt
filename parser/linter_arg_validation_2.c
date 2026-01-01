#include "../include/minirt.h"

void	validate_reflectivity(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " reflectivity invalid.");
	}
	if (!is_valid_float(str) || !check_range(ft_atof(str), 0.0, 1.0))
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " reflectivity invalid.");
	}
}

void	validate_checker_color(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " checker color invalid.");
	}
	if (!check_color_fmt(str))
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " checker color invalid.");
	}
}
