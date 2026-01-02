#include "../include/minirt.h"

static void	lint_plane_extra(char *line, t_scene *scene, size_t count)
{
	size_t	i;
	char	*arg;
	char	*valid_args[7];

	i = 4;
	valid_args[0] = "r=";
	valid_args[1] = "ch=";
	valid_args[2] = "tx=";
	valid_args[3] = "bump=";
	valid_args[4] = "u=";
	valid_args[5] = "v=";
	valid_args[6] = NULL;
	while (i < count)
	{
		arg = index_split(line, i);
		check_arg(arg, valid_args, scene, "Plane");
		i++;
	}
}

void	lint_plane(char *line, t_scene *scene)
{
	size_t	count;

	count = ft_split_inplace(line, ' ');
	if (count < 4 || count > 10)
		erorr(scene, NULL, "Error: Plane args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Plane point invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Plane normal invalid.");
	if (!check_color_fmt(index_split(line, 3)))
		erorr(scene, NULL, "Error: Plane color invalid.");
	lint_plane_extra(line, scene, count);
}
