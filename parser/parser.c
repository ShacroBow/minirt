#include "../minirt.h"

#define BUFFER_SIZE 4096

static char	*read_file_content(int fd)
{
	char	*buffer;
	char	*content;
	char	*temp;
	ssize_t	bytes_read;

	content = safe_malloc(1);
	*content = '\0';
	buffer = safe_malloc(BUFFER_SIZE + 1);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			exit_error("Error: reading scene file");
		buffer[bytes_read] = '\0';
		temp = content;
		content = ft_strjoin(content, buffer);
		free(temp);
	}
	free(buffer);
	return (content);
}

void	parse_line(char *line, t_scene *scene)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens)
	{
		free_tokens(tokens);
		return;
	}
	if (tokens[0] && ft_strncmp(tokens[0], "A", 2) == 0)
		parse_ambient(scene, tokens);
	else if (tokens[0] && ft_strncmp(tokens[0], "C", 2) == 0)
		parse_camera(scene, tokens);
	else if (tokens[0] && ft_strncmp(tokens[0], "L", 2) == 0)
		parse_light(scene, tokens);
	else if (tokens[0] && ft_strncmp(tokens[0], "sp", 3) == 0)
		parse_sphere(scene, tokens);
	else if (tokens[0] && ft_strncmp(tokens[0], "pl", 3) == 0)
		parse_plane(scene, tokens);
	else if (tokens[0] && ft_strncmp(tokens[0], "cy", 3) == 0)
		parse_cylinder(scene, tokens);
	else if (tokens[0][0] != '#' && tokens[0][0] != '\0')
		exit_error("Error: Invalid identifier in scene file.");
	free_tokens(tokens);
}

t_scene	*parse_file(char *filename)
{
	int		fd;
	char	*file_content;
	char	**lines;
	t_scene	*scene;
	int		i;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Error: Cannot open scene file.");
	scene = safe_malloc(sizeof(t_scene));
	*scene = (t_scene){0};
	file_content = read_file_content(fd);
	close(fd);
	lines = ft_split(file_content, '\n');
	free(file_content);
	i = 0;
	while (lines && lines[i])
	{
		parse_line(lines[i], scene);
		i++;
	}
	free_tokens(lines);
	validate_scene(scene);
	return (scene);
}

t_scene *parse_scene(const char *filename)
{
    return parse_file((char *)filename);
}
