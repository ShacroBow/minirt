#include "../include/minirt.h"

static void	parse_cone_struct(char *line, t_cone *co)
{
	parse_vector(index_split(line, 1), &co->center);
	parse_vector(index_split(line, 2), &co->normal);
	co->normal = vec_normalize(co->normal); // DON'T WE ALREADY DECLINE NORMALS THAT ARE NOT NORMALIZED IN LINTING ??????
	co->diameter = ft_atof(index_split(line, 3));
	co->height = ft_atof(index_split(line, 4));
	co->apex = vec_add(co->center, \
		vec_mult(co->normal, co->height / 2));
	co->center = vec_sub(co->center, \
		vec_mult(co->normal, co->height / 2));
}

static void	init_cone_obj(t_object *obj, t_cone *co)
{
	ft_bzero(obj, sizeof(t_object));
	obj->type = CONE;
	obj->shape_data = co;
	obj->reflectivity = 0.0;
	obj->scale_u = 1.0;
	obj->scale_v = 1.0;
	obj->name = ft_strdup("Cone");
	if (!obj->name)
		erorr(NULL, co, "Error: allocation failed.\n");
}

static void	parse_cone_extra_args(char *line, size_t count, t_object *obj, t_scene *scene)
{
	size_t  i;

	i = CONE_MIN_ARGS;
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
	validate_object_extra_args(obj, scene, "Cone");
}

void	parse_cone(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cone		*co;
	size_t		count;

	count = ft_split_inplace(line, ' ');
	co = malloc(sizeof(t_cone));
	if (!co)
		erorr(scene, NULL, "Error: Allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, co, "Error: Allocation failed.\n");
	init_cone_obj(new_obj, co);
	add_object(scene, new_obj);
	parse_cone_struct(line, co);
	parse_vector(index_split(line, 5), &new_obj->color);
	if (count > CONE_MIN_ARGS)
		parse_cone_extra_args(line, count, new_obj, scene);
}
