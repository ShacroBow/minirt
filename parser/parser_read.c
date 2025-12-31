#include "../include/minirt.h"

void	read_file(int fd, char *content, t_scene *scene)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, content, FILE_SIZE + 1);
	if (bytes_read == -1)
	{
		close(fd);
		free_scene(scene);
		exit_error("Error: reading file.");
	}
	if (bytes_read == FILE_SIZE + 1)
	{
		close(fd);
		free_scene(scene);
		exit_error("Error: file too big. Max: 1000000 bytes");
	}
	if (bytes_read == 0)
	{
		close(fd);
		free_scene(scene);
		exit_error("Error: empty file.");
	}
	content[bytes_read] = 0;
}
