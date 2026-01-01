#include "../include/minirt.h"

static void	lint_cylinder_extra(char *line, t_scene *scene, size_t count)
{
	size_t	i;
	char	*arg;
	char	*valid_args[5];

	i = 6;
	valid_args[0] = "r=";
	valid_args[1] = "ch=";
	valid_args[2] = "tx=";
	valid_args[3] = "bump=";
	valid_args[4] = NULL;
	while (i < count)
	{
		arg = index_split(line, i);
		check_arg(arg, valid_args, scene, "Cylinder");
		i++;
	}
}

void	lint_cylinder(char *line, t_scene *scene)
{
	size_t  count;

	count = ft_split_inplace(line, ' ');
	if (count < 6 || count > 12)
		erorr(scene, NULL, "Error: Cylinder args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Cylinder center invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Cylinder normal invalid.");
	if (!is_valid_float(index_split(line, 3))
		|| ft_atof(index_split(line, 3)) <= 0)
		erorr(scene, NULL, "Error: Cylinder diameter invalid.");
	if (!is_valid_float(index_split(line, 4))
		|| ft_atof(index_split(line, 4)) <= 0)
		erorr(scene, NULL, "Error: Cylinder height invalid.");
	if (!check_color_fmt(index_split(line, 5)))
		erorr(scene, NULL, "Error: Cylinder color invalid.");
	if (count >= 7)
		lint_cylinder_extra(line, scene, count);
}
