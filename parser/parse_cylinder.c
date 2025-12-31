#include "../include/minirt.h"

static t_cylinder	*parse_cylinder_struct(t_scene *scene, char *line)
{
	t_cylinder	*cy;

	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		erorr(scene, NULL, "Error: allocation failed.\n");
	parse_vector(index_split(line, 1), &cy->center);
	parse_vector(index_split(line, 2), &cy->normal);
	cy->normal = vec_normalize(cy->normal);
	cy->diameter = ft_atof(index_split(line, 3));
	cy->height = ft_atof(index_split(line, 4));
	return (cy);
}

static void	parse_cylinder_texture(t_object *new_obj, char *line, \
									t_scene *scene)
{
	new_obj->texture = load_ppm(index_split(line, 8));
	if (!new_obj->texture)
		erorr(scene, NULL, "Error: Cylinder texture invalid.");
	new_obj->has_texture = true;
}

static void	parse_cylinder_checker(t_object *new_obj, char *line, \
									t_scene *scene)
{
	if (check_color_fmt(index_split(line, 7)))
	{
		new_obj->has_checkerboard = true;
		parse_vector(index_split(line, 7), &new_obj->checker_color);
	}
	else if (has_extension(index_split(line, 7), ".ppm"))
	{
		new_obj->texture = load_ppm(index_split(line, 7));
		if (!new_obj->texture)
			erorr(scene, NULL, "Error: Cylinder texture invalid.");
		new_obj->has_texture = true;
	}
	else
		erorr(scene, NULL, \
			"Error: Cylinder checker color or texture invalid.");
}

static void	parse_cylinder_extra(t_object *new_obj, char *line, \
								int count, t_scene *scene)
{
	if (count == 8)
		parse_cylinder_checker(new_obj, line, scene);
	else
		new_obj->has_checkerboard = false;
	if (count == 9)
		parse_cylinder_texture(new_obj, line, scene);
}

void	parse_cylinder(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cylinder	*cy;
	int			count;

	count = ft_split_inplace(line, ' ');
	cy = parse_cylinder_struct(scene, line);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, cy, "Error: Allocation failed.\n");
	new_obj->type = CYLINDER;
	new_obj->shape_data = cy;
	new_obj->has_texture = false;
	new_obj->texture = NULL;
	new_obj->has_checkerboard = false;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 5), &new_obj->color);
	if (count >= 7)
		new_obj->reflectivity = ft_atof(index_split(line, 6));
	else
		new_obj->reflectivity = 0.0;
	parse_cylinder_extra(new_obj, line, count, scene);
}
