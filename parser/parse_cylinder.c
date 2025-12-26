#include "../include/minirt.h"

static t_cylinder	*parse_cylinder_struct(t_scene *scene, char *line)
{
	t_cylinder	*cy;

	if (ft_split_inplace(line, ' ') != 6)
		(erorr(scene, NULL, \
			"Error: Incorrect number of arguments for cylinder."));
	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		erorr(scene, NULL, "Error: allocation failed.\n");
	if (!parse_vector(index_split(line, 1), &cy->center))
		erorr(scene, cy, "Error: Invalid vector format for cylinder center.");
	if (!parse_vector(index_split(line, 2), &cy->normal))
		erorr(scene, cy, "Error: Invalid vector format for cylinder normal.");
	validate_normalized_vector(cy->normal, scene);
	cy->normal = vec_normalize(cy->normal);
	cy->diameter = ft_atof(index_split(line, 3));
	cy->height = ft_atof(index_split(line, 4));
	if (cy->diameter <= 0 || cy->height <= 0)
		erorr(scene, cy, "Error: Cylinder diameter and height "
			"must be positive.");
	return (cy);
}

void	parse_cylinder(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cylinder	*cy;

	cy = parse_cylinder_struct(scene, line);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, cy, "Error: Allocation failed.\n");
	new_obj->type = CYLINDER;
	new_obj->shape_data = cy;
	add_object(scene, new_obj);
	if (!parse_color(index_split(line, 5), &new_obj->color))
		erorr(scene, NULL, "Error: Invalid color format for cylinder.");
	validate_color(new_obj->color, scene);
}
