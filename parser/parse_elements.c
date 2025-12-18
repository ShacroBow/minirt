#include "../minirt.h"

void	parse_ambient(t_scene *scene, char *line)
{
	if (scene->ambient_light.is_set)
		(free_scene(scene),
		exit_error("Error: Ambient light can only be declared once."));
	if (ft_split_inplace(line, ' ') != 3)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for ambient light."));
	scene->ambient_light.ratio = ft_atof(index_split(line, 1));
	validate_ratio(scene->ambient_light.ratio, scene);
	if (!parse_color(index_split(line, 2), &scene->ambient_light.color))
		(free_scene(scene),
		exit_error("Error: Invalid color format for ambient light."));
	validate_color(scene->ambient_light.color, scene);
	scene->ambient_light.is_set = true;
}

#include <stdio.h>
void	parse_camera(t_scene *scene, char *line)
{
	if (scene->camera.is_set)
		(free_scene(scene), exit_error("Error: Camera can only be declared once."));
	if (ft_split_inplace(line, ' ') != 4)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for camera."));
	if (!parse_vector(index_split(line, 1), &scene->camera.center))
		(free_scene(scene),
		exit_error("Error: Invalid vector format for camera center."));
	// write(1, ">>>", 3);
	// printf("%s\n", index_split(line, 0));
	// printf("%s\n", index_split(line, 1));
	// printf("%s\n", index_split(line, 2));
	// printf("%s\n", index_split(line, 3));
	// // write(1, line, 19);
	// write(1, "<<<", 3);
	if (!parse_vector(index_split(line, 2), &scene->camera.normal))
		(free_scene(scene),
		exit_error("Error: Invalid vector format for camera normal."));
	validate_normalized_vector(scene->camera.normal, scene);
	scene->camera.normal = vec_normalize(scene->camera.normal);
	scene->camera.fov = ft_atof(index_split(line, 3));
	validate_fov(scene->camera.fov, scene);
	scene->camera.is_set = true;
}

void	parse_light(t_scene *scene, char *line)
{
	t_light	*new_light;
	t_light	*current;

	if (ft_split_inplace(line, ' ') != 4)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for light."));
	new_light = malloc(sizeof(t_light));
	if (!new_light)
		(free_scene(scene), exit_error("Error: allocation failed.\n"));
	if (!parse_vector(index_split(line, 1), &new_light->center))
		(free_scene(scene), free(new_light),
		exit_error("Error: Invalid vector format for light center."));
	new_light->ratio = ft_atof(index_split(line, 2));
	validate_ratio(new_light->ratio, scene);
	if (!parse_color(index_split(line, 3), &new_light->color))
		(free_scene(scene), free(new_light),
		exit_error("Error: Invalid color format for light."));
	validate_color(new_light->color, scene);
	new_light->next = NULL;
	if (!scene->lights)
		scene->lights = new_light;
	else
	{
		current = scene->lights;
		while (current->next)
			current = current->next;
		current->next = new_light;
	}
}

void	parse_sphere(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_sphere	*sp;

	if (ft_split_inplace(line, ' ') != 4)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for sphere."));
	sp = malloc(sizeof(t_sphere));
	if (!sp)
		(free_scene(scene), exit_error("Error: allocation failed.\n"));
	if (!parse_vector(index_split(line, 1), &sp->center))
		(free_scene(scene), free(sp),
		exit_error("Error: Invalid vector format for sphere center."));
	sp->diameter = ft_atof(index_split(line, 2));
	if (sp->diameter <= 0)
		(free_scene(scene), free(sp),
		exit_error("Error: Sphere diameter must be positive."));
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		(free_scene(scene), free(sp), exit_error("Error: allocation failed.\n"));
	new_obj->type = SPHERE;
	new_obj->shape_data = sp;
	if (!parse_color(index_split(line, 3), &new_obj->color))
		(free_scene(scene), free(sp), free(new_obj),
		exit_error("Error: Invalid color format for sphere."));
	validate_color(new_obj->color, scene);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane		*pl;

	if (ft_split_inplace(line, ' ') != 4)
		(free_scene(scene),
		exit_error("Error: Incorrect number of arguments for plane."));
	pl = malloc(sizeof(t_plane));
	if (!pl)
		(free_scene(scene), exit_error("Error: allocation failed.\n"));
	if (!parse_vector(index_split(line, 1), &pl->point))
		(free_scene(scene), free(pl),
		exit_error("Error: Invalid vector format for plane point."));
	if (!parse_vector(index_split(line, 2), &pl->normal))
		(free_scene(scene), free(pl),
		exit_error("Error: Invalid vector format for plane normal."));
	validate_normalized_vector(pl->normal, scene);
	pl->normal = vec_normalize(pl->normal);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		(free_scene(scene), free(pl), exit_error("Error: allocation failed.\n"));
	new_obj->type = PLANE;
	new_obj->shape_data = pl;
	if (!parse_color(index_split(line, 3), &new_obj->color))
		(free_scene(scene), free(pl), free(new_obj),
		exit_error("Error: Invalid color format for plane."));
	validate_color(new_obj->color, scene);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}
