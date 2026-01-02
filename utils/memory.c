#include "../include/minirt.h"

static void	free_objects(t_object *objects)
{
	t_object	*current;
	t_object	*next;

	current = objects;
	while (current)
	{
		next = current->next;
		if (current->has_texture)
			free_texture(current->texture);
		if (current->has_bump)
			free_texture(current->bump);
		free(current->name);
		free(current->shape_data);
		free(current);
		current = next;
	}
}

void	free_scene(t_scene *scene)
{
	if (!scene)
		return ;
	free_objects(scene->objects);
	if (scene->file_content)
		free(scene->file_content);
	if (scene->line_copy)
		free(scene->line_copy);
	free(scene);
}
