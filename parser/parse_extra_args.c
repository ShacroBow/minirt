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
	if (obj->texture)
		erorrf(scene, NULL, obj->name, " multiple textures.");
	obj->texture = load_ppm(str);
	if (!obj->texture)
		erorrf(scene, NULL, obj->name, " texture invalid.");
	obj->has_texture = true;
	obj->texture_enabled = true;
}

void	parse_bumpmap(char *str, t_object *obj, t_scene *scene)
{
	if (obj->bump)
		erorrf(scene, NULL, obj->name, " multiple textures.");
	obj->bump = load_ppm(str);
	if (!obj->bump)
		erorrf(scene, NULL, obj->name, " bump texture invalid.");
	obj->has_bump = true;
	obj->bump_enabled = true;
}

/**
 * Applies one key=value argument (already validated by the linter) to obj.
 */
void	parse_extra_arg(char *arg, t_object *obj, t_scene *scene)
{
	if (ft_strncmp(arg, "r=", 2) == 0)
		parse_reflectivity(arg + 2, obj, scene);
	else if (ft_strncmp(arg, "ch=", 3) == 0)
		parse_checker_color(arg + 3, obj, scene);
	else if (ft_strncmp(arg, "tx=", 3) == 0)
		parse_texture(arg + 3, obj, scene);
	else if (ft_strncmp(arg, "bump=", 5) == 0)
		parse_bumpmap(arg + 5, obj, scene);
	else if (ft_strncmp(arg, "u=", 2) == 0)
		obj->scale_u = ft_atof(arg + 2);
	else if (ft_strncmp(arg, "v=", 2) == 0)
		obj->scale_v = ft_atof(arg + 2);
	else if (ft_strncmp(arg, "tr=", 3) == 0)
		obj->transparency = ft_atof(arg + 3);
	else if (ft_strncmp(arg, "ior=", 4) == 0)
		obj->refractive_index = ft_atof(arg + 4);
	else if (ft_strncmp(arg, "em=", 3) == 0)
		obj->emission = ft_atof(arg + 3);
}
