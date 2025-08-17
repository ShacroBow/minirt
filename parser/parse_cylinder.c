#include "../minirt.h"

void	parse_cylinder(t_scene *scene, char **tokens)
{
	t_object	*new_obj;
	t_cylinder	*cy;

	if (count_tokens(tokens) != 6)
		exit_error("Error: Incorrect number of arguments for cylinder.");
	cy = safe_malloc(sizeof(t_cylinder));
	if (!parse_vector(tokens[1], &cy->center))
		exit_error("Error: Invalid vector format for cylinder center.");
	if (!parse_vector(tokens[2], &cy->normal))
		exit_error("Error: Invalid vector format for cylinder normal.");
	validate_normalized_vector(cy->normal);
	cy->normal = vec_normalize(cy->normal);
	cy->diameter = ft_atof(tokens[3]);
	cy->height = ft_atof(tokens[4]);
	if (cy->diameter <= 0 || cy->height <= 0)
		exit_error("Error: Cylinder diameter and height must be positive.");
	new_obj = safe_malloc(sizeof(t_object));
	new_obj->type = CYLINDER;
	new_obj->shape_data = cy;
	if (!parse_color(tokens[5], &new_obj->color))
		exit_error("Error: Invalid color format for cylinder.");
	validate_color(new_obj->color);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}
