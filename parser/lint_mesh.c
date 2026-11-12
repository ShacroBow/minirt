#include "../include/minirt.h"

void	validate_rotation(char *str, t_scene *scene, const char *obj_name)
{
	if (!check_vector_fmt(str))
		erorrf(scene, NULL, obj_name, " rotation invalid.");
}

static void	lint_mesh_extra(char *line, t_scene *scene, size_t count)
{
	size_t	i;
	char	*valid_args[11];

	valid_args[0] = "r=";
	valid_args[1] = "ch=";
	valid_args[2] = "tx=";
	valid_args[3] = "bump=";
	valid_args[4] = "u=";
	valid_args[5] = "v=";
	valid_args[6] = "tr=";
	valid_args[7] = "ior=";
	valid_args[8] = "rot=";
	valid_args[9] = "em=";
	valid_args[10] = NULL;
	i = MESH_MIN_ARGS;
	while (i < count)
	{
		check_arg(index_split(line, i), valid_args, scene, "Mesh");
		i++;
	}
}

/**
 * obj <file.obj> <center> <size> <color> [rot=x,y,z] [r= ch= tx= em= ...]
 */
void	lint_mesh(char *line, t_scene *scene)
{
	size_t	count;

	count = ft_split_inplace(line, ' ');
	if (count < MESH_MIN_ARGS || count > MESH_MIN_ARGS + 10)
		erorr(scene, NULL, "Error: Mesh args count.");
	if (!has_extension(index_split(line, 1), ".obj"))
		erorr(scene, NULL, "Error: Mesh file must have a .obj extension.");
	if (!check_vector_fmt(index_split(line, 2)))
		erorr(scene, NULL, "Error: Mesh center invalid.");
	if (!is_valid_float(index_split(line, 3))
		|| ft_atof(index_split(line, 3)) <= 0)
		erorr(scene, NULL, "Error: Mesh size invalid.");
	if (!check_color_fmt(index_split(line, 4)))
		erorr(scene, NULL, "Error: Mesh color invalid.");
	if (count > MESH_MIN_ARGS)
		lint_mesh_extra(line, scene, count);
}
