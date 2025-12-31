#include "../include/minirt.h"

void	lint_cylinder(char *line, t_scene *scene)
{
	int	count;

	count = ft_split_inplace(line, ' ');
	if (count < 6 || count > 12)
		erorr(scene, NULL, "Error: Cylinder args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Cylinder center invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Cylinder normal invalid.");
	if (!is_valid_float(index_split(line, 3)) || \
		ft_atof(index_split(line, 3)) <= 0)
		erorr(scene, NULL, "Error: Cylinder diameter invalid.");
	if (!is_valid_float(index_split(line, 4)) || \
		ft_atof(index_split(line, 4)) <= 0)
		erorr(scene, NULL, "Error: Cylinder height invalid.");
	if (!check_color_fmt(index_split(line, 5)))
		erorr(scene, NULL, "Error: Cylinder color invalid.");
	if (count >= 7 && (!is_valid_float(index_split(line, 6)) || \
		!check_range(ft_atof(index_split(line, 6)), 0.0, 1.0)))
		erorr(scene, NULL, "Error: Cylinder reflectivity invalid.");
	if (count == 8 && !check_color_fmt(index_split(line, 7)))
	{
		if (!check_ppm_filename(index_split(line, 7)))
			erorr(scene, NULL, "Error: Cylinder checker color or texture invalid.");
	}
	if (count == 9)
	{
		bool idx7_color = check_color_fmt(index_split(line, 7));
		bool idx7_ppm = check_ppm_filename(index_split(line, 7));
		bool idx8_ppm = check_ppm_filename(index_split(line, 8));
		if (!((idx7_color && idx8_ppm) || (idx7_ppm && idx8_ppm)))
			erorr(scene, NULL, "Error: Cylinder checker color or texture invalid.");
	}
}

void	lint_cone(char *line, t_scene *scene)
{
	int	count;

	count = ft_split_inplace(line, ' ');
	if (count < 6 || count > 12)
		erorr(scene, NULL, "Error: Cone args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Cone center invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Cone normal invalid.");
	if (!is_valid_float(index_split(line, 3)) || \
		ft_atof(index_split(line, 3)) <= 0)
		erorr(scene, NULL, "Error: Cone diameter invalid.");
	if (!is_valid_float(index_split(line, 4)) || \
		ft_atof(index_split(line, 4)) <= 0)
		erorr(scene, NULL, "Error: Cone height invalid.");
	if (!check_color_fmt(index_split(line, 5)))
		erorr(scene, NULL, "Error: Cone color invalid.");
	if (count >= 7 && (!is_valid_float(index_split(line, 6)) || \
		!check_range(ft_atof(index_split(line, 6)), 0.0, 1.0)))
		erorr(scene, NULL, "Error: Cone reflectivity invalid.");
	if (count == 8 && !check_color_fmt(index_split(line, 7)))
	{
		if (!check_ppm_filename(index_split(line, 7)))
			erorr(scene, NULL, "Error: Cone checker color or texture invalid.");
	}
	if (count == 9)
	{
		bool idx7_color = check_color_fmt(index_split(line, 7));
		bool idx7_ppm = check_ppm_filename(index_split(line, 7));
		bool idx8_ppm = check_ppm_filename(index_split(line, 8));
		if (!((idx7_color && idx8_ppm) || (idx7_ppm && idx8_ppm)))
			erorr(scene, NULL, "Error: Cone checker color or texture invalid.");
	}
}
