#include "../include/minirt.h"

static char	*grow(char *buf, size_t len, size_t *cap)
{
	char	*bigger;

	*cap *= 2;
	bigger = malloc(*cap + 1);
	if (bigger)
		ft_memcpy(bigger, buf, len);
	free(buf);
	return (bigger);
}

static char	*read_all(int fd, size_t *len)
{
	char	*buf;
	size_t	cap;
	ssize_t	n;

	cap = 65536;
	buf = malloc(cap + 1);
	*len = 0;
	n = 1;
	while (buf && n > 0)
	{
		if (*len == cap)
			buf = grow(buf, *len, &cap);
		if (buf)
			n = read(fd, buf + *len, cap - *len);
		if (buf && n > 0)
			*len += (size_t)n;
	}
	if (buf && n < 0)
	{
		free(buf);
		buf = NULL;
	}
	if (buf)
		buf[*len] = '\0';
	return (buf);
}

/**
 * Reads a whole file into a NUL terminated buffer of any size.
 * Returns NULL if it can't be opened or read.
 */
char	*read_whole_file(const char *path, size_t *len)
{
	int		fd;
	char	*buf;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	buf = read_all(fd, len);
	close(fd);
	return (buf);
}
