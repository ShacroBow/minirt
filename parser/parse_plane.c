#include "../include/minirt.h"

static void	parse_plane_texture(t_object *new_obj, char *line, t_scene *scene)
{
	new_obj->texture = load_ppm(index_split(line, 6));
	if (!new_obj->texture)
		erorr(scene, NULL, "Error: Plane texture invalid.");
	new_obj->has_texture = true;
}

static void	parse_plane_checker(t_object *new_obj, char *line, t_scene *scene)
{
	if (check_color_fmt(index_split(line, 5)))
	{
		new_obj->has_checkerboard = true;
		parse_vector(index_split(line, 5), &new_obj->checker_color);
	}
	else if (has_extension(index_split(line, 5), ".ppm"))
	{
		new_obj->texture = load_ppm(index_split(line, 5));
		if (!new_obj->texture)
			erorr(scene, NULL, "Error: Plane texture invalid.");
		new_obj->has_texture = true;
	}
	else
		erorr(scene, NULL, \
			"Error: Plane checker color or texture invalid.");
}

static void	parse_plane_extra(t_object *new_obj, char *line, int count, \
								t_scene *scene)
{
	if (count == 6)
		parse_plane_checker(new_obj, line, scene);
	else
		new_obj->has_checkerboard = false;
	if (count == 7)
		parse_plane_texture(new_obj, line, scene);
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane		*pl;
	int			count;

	count = ft_split_inplace(line, ' ');
	pl = malloc(sizeof(t_plane));
	if (!pl)
		erorr(scene, NULL, "Error: Allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, pl, "Error: Allocation failed.\n");
	new_obj->type = PLANE;
	new_obj->shape_data = pl;
	new_obj->has_texture = false;
	new_obj->texture = NULL;
	new_obj->has_checkerboard = false;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 1), &pl->point);
	parse_vector(index_split(line, 2), &pl->normal);
	parse_vector(index_split(line, 3), &new_obj->color);
	if (count >= 5)
		new_obj->reflectivity = ft_atof(index_split(line, 4));
	else
		new_obj->reflectivity = 0.0;
	parse_plane_extra(new_obj, line, count, scene);
}
