#include "../include/minirt.h"

void	lint_cylinder(char *line, t_scene *scene)
{
	int	count;

	count = ft_split_inplace(line, ' ');
	if (count != 6 && count != 7)
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
	if (count == 7 && (!is_valid_float(index_split(line, 6)) || \
		!check_range(ft_atof(index_split(line, 6)), 0.0, 1.0)))
		erorr(scene, NULL, "Error: Cylinder reflectivity invalid.");
}

static void	lint_unique(char *line, t_scene *scene)
{
	if (ft_strncmp(line, "A ", 2) == 0)
	{
		if (scene->ambient_light.is_set)
			erorr(scene, NULL, "Error: Duplicate Ambient.");
		lint_ambient(line, scene);
		scene->ambient_light.is_set = true;
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		if (scene->camera.is_set)
			erorr(scene, NULL, "Error: Duplicate Camera.");
		lint_camera(line, scene);
		scene->camera.is_set = true;
	}
}

void	lint_line(char *line, t_scene *scene)
{
	if (is_ignorable(line))
		return ;
	if (ft_strncmp(line, "A ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		lint_unique(line, scene);
	else if (ft_strncmp(line, "L ", 2) == 0)
		lint_light(line, scene);
	else if (ft_strncmp(line, "sp ", 3) == 0)
		lint_sphere(line, scene);
	else if (ft_strncmp(line, "pl ", 3) == 0)
		lint_plane(line, scene);
	else if (ft_strncmp(line, "cy ", 3) == 0)
		lint_cylinder(line, scene);
	else
		erorr(scene, NULL, "Error: Invalid identifier.");
}

static void	lint_loop(char *line, size_t line_count, t_scene *scene)
{
	size_t	line_len;

	while (line_count--)
	{
		while (*line == 0)
			line += 1;
		line_len = ft_strlen(line);
		lint_line(line, scene);
		line += line_len;
	}
}

void	lint_scene(char *filename, t_scene *scene)
{
	int		fd;
	size_t	line_count;
	char	file_content[FILE_SIZE + 1];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		erorr(scene, NULL, "Error: Cannot open file.");
	read_file(fd, file_content, scene);
	close(fd);
	line_count = ft_split_inplace(file_content, '\n');
	lint_loop(file_content, line_count, scene);
	if (!scene->ambient_light.is_set || !scene->camera.is_set)
		erorr(scene, NULL, "Error: Missing A or C.");
	ft_bzero(scene, sizeof(t_scene));
}
