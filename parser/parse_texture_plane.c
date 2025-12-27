#include "../include/minirt.h"

t_texture_plane	*parse_texture_struct(scene, line)
{
	t_texture_plane	*tx;
	int				fd;
	char			*line;

	fd = open(index_split(line, 3), O_RDONLY);
	if (fd < 0)
		erorr(scene, NULL, "Error: Opening texture plane file.");
	tx = malloc(sizeof(t_texture_plane));
	if (!tx)
		erorr(scene, NULL, "Error: Allocation failed.");
	// TODO // TODO // TODO //
	line = get_next_line(fd);
	handle_comments(line); // See if to implement this
	if (ft_strncmp(line, "P6\n", 3) != 0)
	{
		get_next_line(-1); // MAKE THIS CLEAN get_next_line()'s inner allocation;
		free(line);
		erorr(scene, tx, "Error: Invalid ppm file. Need P6.");
	}
}

void	parse_texture_plane(t_scene *scene, char *line)
{
    t_texture_plane	*tx;
    t_object		*object;

	tx = parse_texture_struct(scene, line);
	object = malloc(sizeof(t_object));
	if (!object)
		erorr(scene, NULL, "Error: Allocation failed.");
	object->color = (t_color){0, 0, 0};
	object->next = NULL;
	object->type = TEXTURE_PLANE;
	object->shape_data = tx;
}