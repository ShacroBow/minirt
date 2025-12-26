#include "../include/minirt.h"

static t_cone	*parse_cone_struct(t_scene *scene, char *line)
{
	t_cone	*co;

	if (ft_split_inplace(line, ' ') != 6)
		(erorr(scene, NULL, \
			"Error: Incorrect number of arguments for cone."));
	co = malloc(sizeof(t_cone));
	if (!co)
		erorr(scene, NULL, "Error: allocation failed.\n");
	if (!parse_vector(index_split(line, 1), &co->center))
		erorr(scene, co, "Error: Invalid vector format for cone center.");
	if (!parse_vector(index_split(line, 2), &co->center_dir))
		erorr(scene, co, "Error: Invalid vector format for cone center_dir.");
	validate_normalized_vector(co->center_dir, scene);
	co->center_dir = vec_normalize(co->center_dir);
	co->diameter = ft_atof(index_split(line, 3));
	co->height = ft_atof(index_split(line, 4));
	co->apex = vec_add(co->center,
			vec_mult(co->center_dir, co->height));
	if (co->diameter <= 0 || co->height <= 0)
		erorr(scene, co, "Error: cone diameter and height "
			"must be positive.");
	return (co);
}

void	parse_cone(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cone		*co;

	co = parse_cone_struct(scene, line);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, co, "Error: Allocation failed.\n");
	new_obj->type = CONE;
	new_obj->shape_data = co;
	add_object(scene, new_obj);
	if (!parse_color(index_split(line, 5), &new_obj->color))
		erorr(scene, NULL, "Error: Invalid color format for cone.");
	validate_color(new_obj->color, scene);
}
