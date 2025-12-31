#include "../include/minirt.h"

void	parse_line(char *line, t_scene *scene)
{
	if (is_ignorable(line))
		return ;
	if (ft_strncmp(line, "A ", 2) == 0)
		parse_ambient(scene, line);
	else if (ft_strncmp(line, "C ", 2) == 0)
		parse_camera(scene, line);
	else if (ft_strncmp(line, "L ", 2) == 0)
		parse_light(scene, line);
	else if (ft_strncmp(line, "sp ", 3) == 0)
		parse_sphere(scene, line);
	else if (ft_strncmp(line, "pl ", 3) == 0)
		parse_plane(scene, line);
	else if (ft_strncmp(line, "cy ", 3) == 0)
		parse_cylinder(scene, line);
	else if (ft_strncmp(line, "co ", 3) == 0)
		parse_cone(scene, line);
	else
		erorr(scene, NULL, "Error: Invalid identifier in scene file.");
}

static void	parse_loop(char *line, size_t line_count, t_scene *scene)
{
	size_t	line_len;

	while (line_count--)
	{
		while (*line == 0)
			line += 1;
		line_len = ft_strlen(line);
		if (!is_ignorable(line))
			parse_line(line, scene);
		line += line_len;
	}
}

static void	parse_file(char *filename, t_scene *scene)
{
	int		fd;
	size_t	line_count;
	char	file_content[FILE_SIZE + 1];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		erorr(scene, NULL, "Error: Cannot open scene file.");
	read_file(fd, file_content, scene);
	line_count = ft_split_inplace(file_content, '\n');
	parse_loop(file_content, line_count, scene);
}

void	parse_scene(const char *filename, t_scene **scene)
{
	long	t;

	*scene = malloc(sizeof(t_scene));
	if (!*scene)
		erorr(NULL, NULL, "Error: Allocation failure.");
	ft_bzero(*scene, sizeof(t_scene));
	if (!DEBUG)
	{
		lint_scene((char *)filename, *scene);
		parse_file((char *)filename, *scene);
	}
	else
	{
		t = get_time_us();
		lint_scene((char *)filename, *scene);
		printf("[DEBUG] Linter: %ldus\n", (get_time_us() - t));
		t = get_time_us();
		parse_file((char *)filename, *scene);
		printf("[DEBUG] Parser: %ldus\n", (get_time_us() - t));
	}
}
