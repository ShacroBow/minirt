#include "../include/minirt.h"

#define BUF_SZ	4096

static bool	is_ignorable(const char *s)
{
	int	i;

	if (!s)
		return (true);
	i = 0;
	while (s[i] && ft_isspace((unsigned char)s[i]))
		i++;
	return (s[i] == '\0' || s[i] == '#');
}

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
	else
	{
		(free_scene(scene), \
		exit_error("Error: Invalid identifier in scene file."));
	}
}

void	read_file(int fd, char *content, t_scene *scene)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, content, FILE_SIZE + 1);
	if (bytes_read == -1)
		(close(fd), free_scene(scene), exit_error("Error: reading file.\n"));
	if (bytes_read == FILE_SIZE + 1)
		(close(fd), free_scene(scene), \
		exit_error("Error: file too big. Max: 1000000 bytes\n"));
	if (bytes_read == 0)
		(close(fd), free_scene(scene), exit_error("Error: empty file.\n"));
	content[bytes_read] = 0;
}

static t_scene	*parse_file(char *filename, t_scene *scene)
{
	int		fd;
	char	*line;
	size_t	line_count;
	size_t	line_len;
	char	file_content[FILE_SIZE + 1];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		(free_scene(scene), exit_error("Error: Cannot open scene file."));
	read_file(fd, file_content, scene);
	line_count = ft_split_inplace(file_content, '\n');
	line = file_content;
	while (line_count--)
	{
		while (*line == 0)
			line += 1;
		line_len = ft_strlen(line);
		if (!is_ignorable(line))
			parse_line(line, scene);
		line += line_len;
	}
	close(fd);
	validate_scene(scene);
	return (scene);
}

t_scene	*parse_scene(const char *filename, t_scene **scene)
{
	*scene = malloc(sizeof(t_scene));
	if (!*scene)
		exit_error("Error: Allocation failure.");
	ft_bzero(*scene, sizeof(t_scene));
	return (parse_file((char *)filename, *scene));
}
