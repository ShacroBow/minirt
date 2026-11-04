#include "../include/minirt.h"

static void	init_plane_obj(t_object *obj, t_plane *pl, t_scene *scene)
{
	obj->type = PLANE;
	obj->shape_data = pl;
	obj->reflectivity = 0.0;
	obj->scale_u = 1.0;
	obj->scale_v = 1.0;
	obj->refractive_index = 1.5;
	obj->name = ft_strdup("Plane");
	if (!obj->name)
		erorr(scene, NULL, "Error: allocation failed.");
}

static void	parse_plane_extra_args(char *line, size_t count, t_object *obj,
				t_scene *scene)
{
	size_t	i;
	char	*arg;

	i = PLANE_MIN_ARGS;
	while (i < count)
	{
		arg = index_split(line, i);
		parse_extra_arg(arg, obj, scene);
		i++;
	}
	validate_object_extra_args(obj, scene, "Plane");
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane		*pl;
	size_t		count;

	count = ft_split_inplace(line, ' ');
	pl = malloc(sizeof(t_plane));
	if (!pl)
		erorr(scene, NULL, "Error: allocation failed.");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, pl, "Error: allocation failed.");
	ft_bzero(new_obj, sizeof(t_object));
	add_object(scene, new_obj);
	init_plane_obj(new_obj, pl, scene);
	if (!parse_vector(index_split(line, 1), &pl->point)
		|| !parse_vector(index_split(line, 2), &pl->normal)
		|| !parse_vector(index_split(line, 3), &new_obj->color))
		erorr(scene, NULL, "Error parsing vector plane");
	if (count > PLANE_MIN_ARGS)
		parse_plane_extra_args(line, count, new_obj, scene);
}
