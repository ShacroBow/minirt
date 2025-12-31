#include "../include/minirt.h"

static void	free_lights(t_light *lights)
{
	t_light	*current;
	t_light	*next;

	current = lights;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

static void	free_objects(t_object *objects)
{
	t_object	*current;
	t_object	*next;

	current = objects;
	while (current)
	{
		next = current->next;
		if (current->has_texture && current->texture)
			free_texture(current->texture);
		if (current->has_bump && current->bump)
			free_texture(current->bump);
		free(current->shape_data);
		free(current);
		current = next;
	}
}

void	free_scene(t_scene *scene)
{
	if (!scene)
		return ;
	free_lights(scene->lights);
	free_objects(scene->objects);
	free(scene);
}
