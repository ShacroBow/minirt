#include "../include/mesh.h"

/**
 * Returns the next line of the buffer at *cursor, NUL terminated in place
 * (a trailing '\r' of Windows line endings is removed too).
 */
char	*next_line(char **cursor)
{
	char	*start;
	char	*p;

	start = *cursor;
	if (!start || *start == '\0')
		return (NULL);
	p = start;
	while (*p && *p != '\n')
		p++;
	*cursor = p;
	if (*p == '\n')
	{
		*p = '\0';
		*cursor = p + 1;
	}
	if (p > start && *(p - 1) == '\r')
		*(p - 1) = '\0';
	return (start);
}

/**
 * Path of name relative to the directory of file ("models/a.obj" + "a.mtl"
 * gives "models/a.mtl"). Windows style backslashes are turned into slashes.
 */
char	*path_in_dir(const char *file, const char *name)
{
	const char	*slash;
	size_t		dir_len;
	char		*out;
	size_t		i;

	slash = ft_strrchr(file, '/');
	dir_len = 0;
	if (slash && name[0] != '/')
		dir_len = (size_t)(slash - file) + 1;
	out = malloc(dir_len + ft_strlen(name) + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, file, dir_len);
	ft_memcpy(out + dir_len, name, ft_strlen(name) + 1);
	i = dir_len;
	while (out[i])
	{
		if (out[i] == '\\')
			out[i] = '/';
		i++;
	}
	return (out);
}
