#include "../include/minirt.h"

static bool	has_null_byte(const char *content, ssize_t size)
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
