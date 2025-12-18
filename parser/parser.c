#include "../minirt.h"

# define BUF_SZ 4096

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

static char	*read_line(int fd)
{
	char		*line;
	char		buf[2];
	ssize_t		r;
	size_t		len;

	line = NULL;
	len = 0;
	buf[1] = '\0';
	r = 1;
	while (r > 0)
	{
		r = read(fd, buf, 1);
		if (r <= 0)
			break ;
		if (buf[0] == '\n')
			break ;
		line = (char *)ft_realloc(line, len, len + 1, sizeof(char));
		if (!line)
			return (NULL);
		line[len] = buf[0];
		len++;
	}
	if (!line && r == 0)
		return (NULL);
	line = (char *)ft_realloc(line, len, len + 1, sizeof(char));
	if (!line)
		return (NULL);
	line[len] = '\0';
	return (line);
}

void	parse_line(char *line, t_scene *scene)
{
	char	**tokens;

	if (is_ignorable(line))
		return ;
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
	{
		free_tokens(tokens);
		return ;
	}
	if (ft_strncmp(tokens[0], "A", 2) == 0)
		parse_ambient(scene, tokens);
	else if (ft_strncmp(tokens[0], "C", 2) == 0)
		parse_camera(scene, tokens);
	else if (ft_strncmp(tokens[0], "L", 2) == 0)
		parse_light(scene, tokens);
	else if (ft_strncmp(tokens[0], "sp", 3) == 0)
		parse_sphere(scene, tokens);
	else if (ft_strncmp(tokens[0], "pl", 3) == 0)
		parse_plane(scene, tokens);
	else if (ft_strncmp(tokens[0], "cy", 3) == 0)
		parse_cylinder(scene, tokens);
	else
		exit_error("Error: Invalid identifier in scene file.");
	free_tokens(tokens);
}

ssize_t	create_file_lines(int fd)
{
	char	file_content[1000001];
	ssize_t	bytes_read;
	ssize_t	line_count;
	size_t	line_len;

	line_count = 0;
	line_len = 0;
	bytes_read = read(fd, file_content, 1000001);
	if (bytes_read == -1)
		return (-1);
	if (bytes_read > 1000000)
		return (-1);
	file_content[bytes_read] = 0;
	while (1)
	{
		if (!ft_strchr(file_content + line_len, '\n'))
			break ;
		line_len = ft_strchr(file_content, '\n') - file_content + 1;
		file_content[line_len - 1] = 0;
		line_count++;
	}

	return (line_count);
}

static t_scene	*parse_file(char *filename, t_scene *scene)
{
	int		fd;
	char	**lines;
	size_t	i;

	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Error: Cannot open scene file.");
	lines = create_file_lines(fd);
	if (!lines)
		(close(fd), exit_error("Error: Reading file content."));
	while (1)
	{

		if (!is_ignorable(line))
			parse_line(line, scene);
		free(line);
	}
	close(fd);
	validate_scene(scene);
	return (scene);
}

t_scene	*parse_scene(const char *filename, t_scene *scene)
{
	return (parse_file((char *)filename, scene));
}
