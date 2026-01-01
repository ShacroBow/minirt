#include "../include/minirt.h"

static void	init_plane_obj(t_object *obj, t_plane *pl)
{
	ft_bzero(obj, sizeof(t_object));
	obj->type = PLANE;
	obj->shape_data = pl;
	obj->reflectivity = 0.0;
	obj->scale_u = 1.0;
	obj->scale_v = 1.0;
	obj->name = ft_strdup("Plane");
	if (!obj->name)
		erorr(NULL, pl, "Error: allocation failed.\n");
}

static void	parse_plane_extra_args(char *line, size_t count, t_object *obj, t_scene *scene)
{
	size_t  i;

	i = PLANE_MIN_ARGS;
	while (i < count)
	{
		char *arg = index_split(line, i);
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
		i++;
	}
	validate_object_extra_args(obj, scene, "Plane");
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane     *pl;
	size_t		count;

	count = ft_split_inplace(line, ' ');
	pl = malloc(sizeof(t_plane));
	if (!pl)
		erorr(scene, NULL, "Error: allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, pl, "Error: allocation failed.\n");
	init_plane_obj(new_obj, pl);
	add_object(scene, new_obj);
	parse_vector(index_split(line, 1), &pl->point);
	parse_vector(index_split(line, 2), &pl->normal);
	parse_vector(index_split(line, 3), &new_obj->color);
	if (count > PLANE_MIN_ARGS)
		parse_plane_extra_args(line, count, new_obj, scene);
}
