#include "../include/mesh.h"

static void	init_mesh_obj(t_object *obj, t_mesh *mesh, t_scene *scene)
{
	obj->type = MESH;
	obj->shape_data = mesh;
	obj->scale_u = 1.0;
	obj->scale_v = 1.0;
	obj->refractive_index = 1.5;
	obj->name = ft_strdup("Mesh");
	if (!obj->name)
		erorr(scene, NULL, "Error: allocation failed.");
	mesh_set_rotation(mesh, (t_vec3){0.0, 0.0, 0.0});
}

static void	parse_mesh_extra_args(char *line, size_t count, t_object *obj,
				t_scene *scene)
{
	size_t	i;
	char	*arg;
	t_vec3	degrees;

	i = MESH_MIN_ARGS;
	while (i < count)
	{
		arg = index_split(line, i);
		if (ft_strncmp(arg, "rot=", 4) == 0)
		{
			if (!parse_vector(arg + 4, &degrees))
				erorr(scene, NULL, "Error: Mesh rotation invalid.");
			mesh_set_rotation((t_mesh *)obj->shape_data, degrees);
		}
		else
			parse_extra_arg(arg, obj, scene);
		i++;
	}
	validate_object_extra_args(obj, scene, "Mesh");
}

/**
 * obj <file.obj> <center> <size> <color> [extra args]
 * size is the length of the model's longest side in the scene.
 */
void	parse_mesh(t_scene *scene, char *line)
{
	t_object	*obj;
	t_mesh		*mesh;
	size_t		count;

	count = ft_split_inplace(line, ' ');
	mesh = ft_calloc(1, sizeof(t_mesh));
	if (!mesh)
		erorr(scene, NULL, "Error: allocation failed.");
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
		erorr(scene, mesh, "Error: allocation failed.");
	add_object(scene, obj);
	init_mesh_obj(obj, mesh, scene);
	if (!parse_vector(index_split(line, 2), &mesh->center)
		|| !parse_vector(index_split(line, 4), &obj->color))
		erorr(scene, NULL, "Error parsing vector mesh");
	mesh->size = ft_atof(index_split(line, 3));
	if (count > MESH_MIN_ARGS)
		parse_mesh_extra_args(line, count, obj, scene);
	load_obj(scene, mesh, index_split(line, 1));
}
