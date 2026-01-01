#include "../include/minirt.h"

static void	init_cylinder_obj(t_object *obj, t_cylinder *cy)
{
	ft_bzero(obj, sizeof(t_object));
	obj->type = CYLINDER;
	obj->shape_data = cy;
	obj->reflectivity = 0.0;
	obj->scale_u = 1.0;
	obj->scale_v = 1.0;
	obj->name = ft_strdup("Cylinder");
	if (!obj->name)
		erorr(NULL, cy, "Error: allocation failed.\n"); // COULD BE TROUBLE FREEING cy HERE ???!!!!!!!!! double free?
}

static void	parse_cylinder_struct(char *line, t_cylinder *cy)
{
	parse_vector(index_split(line, 1), &cy->center);
	parse_vector(index_split(line, 2), &cy->normal);
	cy->normal = vec_normalize(cy->normal);
	cy->diameter = ft_atof(index_split(line, 3));
	cy->height = ft_atof(index_split(line, 4));
}

static void	parse_cylinder_extra_args(char *line, size_t count, t_object *obj, t_scene *scene)
{
	size_t  i;

	i = CYLINDER_MIN_ARGS;
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
	validate_object_extra_args(obj, scene, "Cylinder");
}

void	parse_cylinder(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cylinder	*cy;
	size_t		count;

	count = ft_split_inplace(line, ' ');
	cy = malloc(sizeof(t_cylinder));
	if (!cy)
		erorr(scene, NULL, "Error: Allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, cy, "Error: Allocation failed.\n");
	init_cylinder_obj(new_obj, cy);
	add_object(scene, new_obj);
	parse_cylinder_struct(line, cy);
	parse_vector(index_split(line, 5), &new_obj->color);
	if (count > CYLINDER_MIN_ARGS)
		parse_cylinder_extra_args(line, count, new_obj, scene);
}