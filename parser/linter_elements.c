#include "../include/minirt.h"

void	lint_ambient(char *line, t_scene *scene)
{
	if (ft_split_inplace(line, ' ') != 3)
		erorr(scene, NULL, "Error: Ambient light args count.");
	if (!is_valid_float(index_split(line, 1)) || \
		!check_range(ft_atof(index_split(line, 1)), 0.0, 1.0))
		erorr(scene, NULL, "Error: Ambient light ratio invalid.");
	if (!check_color_fmt(index_split(line, 2)))
		erorr(scene, NULL, "Error: Ambient light color invalid.");
}

void	lint_camera(char *line, t_scene *scene)
{
	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Camera args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Camera center invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Camera normal invalid.");
	if (!is_valid_float(index_split(line, 3)) || \
		!check_range(ft_atof(index_split(line, 3)), 0, 180))
		erorr(scene, NULL, "Error: Camera FOV invalid.");
}

void	lint_light(char *line, t_scene *scene)
{
	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Light args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Light center invalid.");
	if (!is_valid_float(index_split(line, 2)) || \
		!check_range(ft_atof(index_split(line, 2)), 0.0, 1.0))
		erorr(scene, NULL, "Error: Light ratio invalid.");
	if (!check_color_fmt(index_split(line, 3)))
		erorr(scene, NULL, "Error: Light color invalid.");
}

void	lint_sphere(char *line, t_scene *scene)
{
	int	count;

	count = ft_split_inplace(line, ' ');
	if (count < 4 || count > 7)
		erorr(scene, NULL, "Error: Sphere args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Sphere center invalid.");
	if (!is_valid_float(index_split(line, 2)) || \
		ft_atof(index_split(line, 2)) <= 0)
		erorr(scene, NULL, "Error: Sphere diameter invalid.");
	if (!check_color_fmt(index_split(line, 3)))
		erorr(scene, NULL, "Error: Sphere color invalid.");
	if (count >= 5 && (!is_valid_float(index_split(line, 4)) || \
		!check_range(ft_atof(index_split(line, 4)), 0.0, 1.0)))
		erorr(scene, NULL, "Error: Sphere reflectivity invalid.");
	if (count == 6 && !check_color_fmt(index_split(line, 5)))
		if (!check_ppm_filename(index_split(line, 5)))
			erorr(scene, NULL, "Error: Sphere checker color or texture invalid.");
	if (count == 7 && (!check_color_fmt(index_split(line, 5)) || !check_ppm_filename(index_split(line, 6))))
		erorr(scene, NULL, "Error: Sphere checker color or texture invalid.");
}

void	lint_plane(char *line, t_scene *scene)
{
	int	count;

	count = ft_split_inplace(line, ' ');
	if (count < 4 || count > 6 + 1)
		erorr(scene, NULL, "Error: Plane args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Plane point invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Plane normal invalid.");
	if (!check_color_fmt(index_split(line, 3)))
		erorr(scene, NULL, "Error: Plane color invalid.");
	if (count >= 5 && (!is_valid_float(index_split(line, 4)) || \
		!check_range(ft_atof(index_split(line, 4)), 0.0, 1.0)))
		erorr(scene, NULL, "Error: Plane reflectivity invalid.");
	if (count == 6 && !check_color_fmt(index_split(line, 5)))
	{
		if (!check_ppm_filename(index_split(line, 5)))
			erorr(scene, NULL, "Error: Plane checker color or texture invalid.");
	}
	if (count == 7 && (!check_color_fmt(index_split(line, 5)) || !check_ppm_filename(index_split(line, 6))))
		erorr(scene, NULL, "Error: Plane checker color or texture invalid.");
}
