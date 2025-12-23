#include "../minirt.h"

void	parse_cone(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cone		*co;

	if (ft_split_inplace(line, ' ') != 6)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for cone."));
	co = malloc(sizeof(t_cone));
	if (!co)
		(free_scene(scene), exit_error("Error: allocation failed.\n"));
	if (!parse_vector(index_split(line, 1), &co->center))
		(free_scene(scene), free(co),
		exit_error("Error: Invalid vector format for cone center."));
	if (!parse_vector(index_split(line, 2), &co->normal))
		(free_scene(scene), free(co),
		exit_error("Error: Invalid vector format for cone normal."));
	validate_normalized_vector(co->normal, scene);
	co->normal = vec_normalize(co->normal);
	co->diameter = ft_atof(index_split(line, 3));
	co->height = ft_atof(index_split(line, 4));
	if (co->diameter <= 0 || co->height <= 0)
		(free_scene(scene), free(co),
		exit_error("Error: Cone diameter and height must be positive."));
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		(free_scene(scene), free(co),
		exit_error("Error: Allocation failed.\n"));
	new_obj->type = CONE;
	new_obj->shape_data = co;
	if (!parse_color(index_split(line, 5), &new_obj->color))
		(free_scene(scene), free(co), free(new_obj),
		exit_error("Error: Invalid color format for cone."));
	validate_color(new_obj->color, scene);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}