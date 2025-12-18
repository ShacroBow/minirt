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
	// tokens = ft_split(line, ' ');
	// if (!tokens || !tokens[0])
	// {
	// 	free_tokens(tokens);
	// 	return ;
	// }
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
		exit_error("Error: Invalid identifier in scene file.");
	// free_tokens(tokens);
}

int	read_file(int fd, char *content)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, content, FILE_SIZE + 1);
	if (bytes_read == -1)
		return (write(2, "Error: reading file.\n", 20), 1);
	if (bytes_read == FILE_SIZE + 1)
		return (write(2, "Error: file too big. Max: 1000000 bytes\n", 40), 1);
	if (bytes_read == 0)
		return (write(2, "Error: empty file.\n", 19), 1);
	content[bytes_read] = 0;
	return (0);
}

char	*next_line(char *content)
{
	char	*line;

	while (*content != 0)
}

void	skip_nulls(char **content)
{
	while (**content == 0)
		(*content += 1);
}

static t_scene	*parse_file(char *filename, t_scene *scene)
{
	int		fd;
	char	*line;
	size_t	line_count;
	size_t	i;
	char	file_content[FILE_SIZE + 1];

	i = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Error: Cannot open scene file.");
	if (read_file(fd, file_content))
		return (free_scene(scene), NULL);
	line_count = ft_split_inplace(file_content, '\n');
	line = file_content;
	while (i < line_count)
	{
		while (*line == 0)
			line += 1;
		if (!is_ignorable(line))
			parse_line(line, scene);
		line += ft_strlen(line); // SINCE parse_line() also uses ft_split_inplace(), resulting ft_strlen() here will NOT be correct intended value << CHANGE THIS
		i++;
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
