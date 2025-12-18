#include "../minirt.h"

void	parse_cylinder(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cylinder	*cy;

	if (ft_split_inplace(line, ' ') != 6)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for cylinder."));
	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		(free_scene(scene), exit_error("Error: allocation failed.\n"));
	if (!parse_vector(index_split(line, 1), &cy->center))
		(free_scene(scene), free(cy),
		exit_error("Error: Invalid vector format for cylinder center."));
	if (!parse_vector(index_split(line, 2), &cy->normal))
		(free_scene(scene), free(cy),
		exit_error("Error: Invalid vector format for cylinder normal."));
	validate_normalized_vector(cy->normal, scene);
	cy->normal = vec_normalize(cy->normal);
	cy->diameter = ft_atof(index_split(line, 3));
	cy->height = ft_atof(index_split(line, 4));
	if (cy->diameter <= 0 || cy->height <= 0)
		(free_scene(scene), free(cy),
		exit_error("Error: Cylinder diameter and height must be positive."));
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		(free_scene(scene), free(cy),
		exit_error("Error: Allocation failed.\n"));
	new_obj->type = CYLINDER;
	new_obj->shape_data = cy;
	if (!parse_color(index_split(line, 5), &new_obj->color))
		(free_scene(scene), free(cy), free(new_obj),
		exit_error("Error: Invalid color format for cylinder."));
	validate_color(new_obj->color, scene);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}
