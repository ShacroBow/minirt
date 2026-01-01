#include "../include/minirt.h"

static int	read_file_to_buf(const char *path, char *buf)
{
	int		fd;
	ssize_t	n;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	n = read(fd, buf, TEXTURE_FILE_SIZE);
	close(fd);
	if (n <= 0)
		return (-1);
	buf[n] = 0;
	return (n);
}

static t_texture	*create_texture(int w, int h, unsigned char *data)
{
	t_texture	*tex;

	tex = malloc(sizeof(t_texture));
	if (!tex)
	{
		free(data);
		return (NULL);
	}
	tex->width = w;
	tex->height = h;
	tex->channels = 3;
	tex->data = data;
	return (tex);
}

static t_texture	*parse_ppm_buf(char *buf, int n)
{
	int				i;
	int				dims[2];
	unsigned char	*data;

	i = 0;
	if (!ppm_header(buf, &i, &dims[0], &dims[1]))
		return (NULL);
	if (dims[0] <= 0 || dims[1] <= 0
		|| (size_t)n < (size_t)i + (size_t)dims[0] * dims[1] * 3)
		return (NULL);
	data = malloc((size_t)dims[0] * dims[1] * 3);
	if (!data)
		return (NULL);
	ft_memcpy(data, buf + i, (size_t)dims[0] * dims[1] * 3);
	return (create_texture(dims[0], dims[1], data));
}

t_texture	*load_ppm(const char *path)
{
	char		*buf;
	int			n;
	t_texture	*tex;

	buf = malloc(TEXTURE_FILE_SIZE + 1);
	if (!buf)
		return (NULL);
	n = read_file_to_buf(path, buf);
	if (n < 0)
	{
		free(buf);
		return (NULL);
	}
	tex = parse_ppm_buf(buf, n);
	free(buf);
	return (tex);
}

void	free_texture(t_texture *tex)
{
	if (!tex)
		return ;
	if (tex->data)
		free(tex->data);
	free(tex);
}
