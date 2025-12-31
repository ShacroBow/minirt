#include "../include/minirt.h"

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
	else if (ft_strncmp(line, "co ", 3) == 0)
		lint_cone(line, scene);
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
		scene->line_copy = ft_strdup(line);
		if (!scene->line_copy)
			erorr(scene, NULL, "Error: Allocation failure.");
		lint_line(scene->line_copy, scene);
		free(scene->line_copy);
		scene->line_copy = NULL;
		line += line_len;
	}
}

void	lint_scene(char *file_content, size_t line_count, t_scene *scene)
{
	char	*saved_file_content;

	saved_file_content = scene->file_content;
	scene->line_copy = NULL;
	lint_loop(file_content, line_count, scene);
	if (!scene->ambient_light.is_set || !scene->camera.is_set)
		erorr(scene, NULL, "Error: Missing A or C.");
	ft_bzero(scene, sizeof(t_scene));
	scene->file_content = saved_file_content;
}
