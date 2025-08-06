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
		{
			free(content);
			free(buffer);
			exit_error("Error: Could not read from file.");
		}
		buffer[bytes_read] = '\0';
		temp = safe_malloc(ft_strlen(content) + ft_strlen(buffer) + 1);
		sprintf(temp, "%s%s", content, buffer);
		free(content);
		content = temp;
	}
	free(buffer);
	return (content);
}

void	parse_line(char *line, t_scene *scene)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens ||!tokens)
	{
		free_tokens(tokens);
		return ;
	}
	if (ft_strncmp(tokens, "A", 2) == 0)
		parse_ambient(scene, tokens);
	else if (ft_strncmp(tokens, "C", 2) == 0)
		parse_camera(scene, tokens);
	else if (ft_strncmp(tokens, "L", 2) == 0)
		parse_light(scene, tokens);
	else if (ft_strncmp(tokens, "sp", 3) == 0)
		parse_sphere(scene, tokens);
	else if (ft_strncmp(tokens, "pl", 3) == 0)
		parse_plane(scene, tokens);
	else if (ft_strncmp(tokens, "cy", 3) == 0)
		parse_cylinder(scene, tokens);
	else if (tokens!= '#' && tokens!= '\n')
		exit_error("Error: Unknown identifier in scene file.");
	free_tokens(tokens);
}

t_scene	*parse_scene(const char *filename)
{
	int		fd;
	char	*file_content;
	char	**lines;
	int		i;
	t_scene	*scene;

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