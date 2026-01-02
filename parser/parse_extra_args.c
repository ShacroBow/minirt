#include "../include/minirt.h"

void	parse_reflectivity(char *str, t_object *obj, t_scene *scene)
{
	(void)scene;
	obj->reflectivity = ft_atof(str);
	obj->reflectivity_enabled = true;
}

void	parse_checker_color(char *str, t_object *obj, t_scene *scene)
{
	if (check_color_fmt(str))
	{
		obj->has_checkerboard = true;
		obj->checker_enabled = true;
		if (!parse_vector(str, &obj->checker_color))
			erorr(scene, NULL, "Error\n");
	}
	else
		erorrf(scene, NULL, obj->name, " checker color or texture invalid.");
}

void	parse_texture(char *str, t_object *obj, t_scene *scene)
{
	obj->texture = load_ppm(str);
	if (!obj->texture)
		erorrf(scene, NULL, obj->name, " texture invalid.");
	obj->has_texture = true;
	obj->texture_enabled = true;
}

void	parse_bumpmap(char *str, t_object *obj, t_scene *scene)
{
	obj->bump = load_ppm(str);
	if (!obj->bump)
		erorrf(scene, NULL, obj->name, " bump texture invalid.");
	obj->has_bump = true;
	obj->bump_enabled = true;
}
