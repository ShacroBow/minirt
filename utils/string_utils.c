#include "../include/minirt.h"

bool	has_extension(const char *filename, const char *ext)
{
	size_t	len;
	size_t	ext_len;

	if (!filename || !ext)
		return (false);
	len = ft_strlen(filename);
	ext_len = ft_strlen(ext);
	if (len < ext_len)
		return (false);
	return (ft_strncmp(filename + (len - ext_len), ext, ext_len) == 0);
}
