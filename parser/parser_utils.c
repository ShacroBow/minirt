#include "../include/minirt.h"

static bool	split3_in_place(char *str, char **a, char **b, char **c)
{
	char	*p1;
	char	*p2;

	if (!str)
		return (false);
	p1 = ft_strchr(str, ',');
	if (!p1)
		return (false);
	*p1 = '\0';
	*b = p1 + 1;
	p2 = ft_strchr(*b, ',');
	if (!p2)
		return (false);
	*p2 = '\0';
	*c = p2 + 1;
	if (ft_strchr(*c, ','))
		return (false);
	*a = str;
	return (true);
}

static bool	ft_valid_3str(char *str)
{
	int	commas;
	int	i;

	if (!str)
		return (false);
	commas = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			commas++;
		else if (!(ft_isspace(str[i]) || ft_isdigit(str[i])
				|| str[i] == '.' || str[i] == '+' || str[i] == '-'))
			return (false);
		i++;
	}
	return (commas == 2);
}

bool	parse_vector(char *str, t_vec3 *vec)
{
	char	*a;
	char	*b;
	char	*c;
	char	*tmp_str;

	if (!ft_valid_3str(str))
		return (false);
	tmp_str = ft_strdup(str);
	if (!tmp_str)
		return (write(2, "Error: allocation failed\n", 25), false);
	if (!split3_in_place(tmp_str, &a, &b, &c))
	{
		free(tmp_str);
		return (false);
	}
	vec->x = ft_atof(a);
	vec->y = ft_atof(b);
	vec->z = ft_atof(c);
	free(tmp_str);
	return (true);
}

bool	check_ppm_filename(char *str)
{
	int len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	if (len < 5)
		return (false);
	if (ft_strcmp(str + len - 4, ".ppm") == 0)
		return (true);
	return (false);
}

bool	is_ignorable(const char *s)
{
	int	i;

	if (!s)
		return (true);
	i = 0;
	while (s[i] && ft_isspace((unsigned char)s[i]))
		i++;
	return (s[i] == '\0' || s[i] == '#');
}

bool	has_null_byte(const char *content, ssize_t size)
{
	ssize_t	i;

	i = 0;
	while (i < size)
	{
		if (content[i] == '\0')
			return (true);
		i++;
	}
	return (false);
}

void	read_file(int fd, char *content, t_scene *scene)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, content, FILE_SIZE + 1);
	close(fd);
	if (bytes_read == -1)
		erorr(scene, NULL, "Error: reading scene file.\n");
	if (bytes_read == FILE_SIZE + 1)
		erorr(scene, NULL, "Error: scene file too big.\n");
	if (bytes_read == 0)
		erorr(scene, NULL, "Error: empty scene file.\n");
	if (has_null_byte(content, bytes_read))
		erorr(scene, NULL, "Error: Null byte in scene file.\n");
	content[bytes_read] = 0;
}

double	compute_uv_scale(double size_m, int tex_dim_pixels)
{
	if (tex_dim_pixels <= 0)
		return (1.0);
	/* Adjusted heuristic to reduce excessive repeats for typical textures.
	   Use a larger divisor so a 512px texture on a ~1m object doesn't repeat
	   many times by default. Also allow scales < 1.0 so textures can stretch
	   when appropriate, but clamp to a small positive lower bound. */
	// double scale = size_m * (double)tex_dim_pixels / 1000.0;
	// if (scale < 0.25)
	// 	return (0.25);
	(void)size_m;
	double scale = 0.4;
	return (scale);
}
