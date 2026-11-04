#include "../include/minirt.h"

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

static t_texture	*parse_ppm_buf(char *buf, size_t n)
{
	int				i;
	int				dims[2];
	unsigned char	*data;

	i = 0;
	if (!ppm_header(buf, &i, &dims[0], &dims[1]))
		return (NULL);
	if (dims[0] <= 0 || dims[1] <= 0
		|| n < (size_t)i + (size_t)dims[0] * dims[1] * 3)
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
	size_t		n;
	t_texture	*tex;

	buf = read_whole_file(path, &n);
	if (!buf)
		return (NULL);
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
