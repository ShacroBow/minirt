#include "../include/minirt.h"

// to make this file norm compliant. it was heavily.
// if some stuff looks weird it cuz of that.

void	parse_ambient(t_scene *scene, char *line)
{
	if (scene->ambient_light.is_set)
		erorr(scene, NULL, "Error: Ambient light can only be declared once.");
	if (ft_split_inplace(line, ' ') != 3)
		erorr(scene, NULL, \
			"Error: Incorrect number of arguments for ambient light.");
	scene->ambient_light.ratio = ft_atof(index_split(line, 1));
	validate_ratio(scene->ambient_light.ratio, scene);
	if (!parse_color(index_split(line, 2), &scene->ambient_light.color))
		erorr(scene, NULL, "Error: Invalid color format for ambient light.");
	validate_color(scene->ambient_light.color, scene);
	scene->ambient_light.is_set = true;
}

void	parse_camera(t_scene *scene, char *line)
{
	if (scene->camera.is_set)
		erorr(scene, NULL, "Error: Camera can only be declared once.");
	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Incorrect number of arguments for camera.");
	if (!parse_vector(index_split(line, 1), &scene->camera.center))
		erorr(scene, NULL, "Error: Invalid vector format for camera center.");
	if (!parse_vector(index_split(line, 2), &scene->camera.normal))
		erorr(scene, NULL, "Error: Invalid vector format for camera normal.");
	validate_normalized_vector(scene->camera.normal, scene);
	scene->camera.normal = vec_normalize(scene->camera.normal);
	scene->camera.fov = ft_atof(index_split(line, 3));
	validate_fov(scene->camera.fov, scene);
	scene->camera.is_set = true;
}

//nlight == new_light btw.
void	parse_light(t_scene *scene, char *line)
{
	t_light	*nlight;
	t_light	*current;

	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Incorrect number of arguments for light.");
	nlight = malloc(sizeof(t_light));
	if (!nlight)
		erorr(scene, NULL, "Error: allocation failed.\n");
	if (!parse_vector(index_split(line, 1), &nlight->center))
		erorr(scene, nlight, "Error: Invalid vector format for light center.");
	nlight->ratio = ft_atof(index_split(line, 2));
	validate_ratio(nlight->ratio, scene);
	if (!parse_color(index_split(line, 3), &nlight->color))
		erorr(scene, nlight, "Error: Invalid color format for light.");
	validate_color(nlight->color, scene);
	nlight->next = NULL;
	if (!scene->lights)
		scene->lights = nlight;
	else
	{
		current = scene->lights;
		while (current->next)
			current = current->next;
		current->next = nlight;
	}
}

void	parse_sphere(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_sphere	*sp;

	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Incorrect number of arguments for sphere.");
	sp = malloc(sizeof(t_sphere));
	if (!sp)
		erorr(scene, NULL, "Error: allocation failed.\n");
	if (!parse_vector(index_split(line, 1), &sp->center))
		erorr(scene, sp, "Error: Invalid vector format for sphere center.");
	sp->diameter = ft_atof(index_split(line, 2));
	if (sp->diameter <= 0)
		erorr(scene, sp, "Error: Sphere diameter must be positive.");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, sp, "Error: allocation failed.\n");
	new_obj->type = SPHERE;
	new_obj->shape_data = sp;
	if (!parse_color(index_split(line, 3), &new_obj->color))
		erorr(scene, (free(new_obj), sp), \
		"Error: Invalid color format for sphere.");
	validate_color(new_obj->color, scene);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane		*pl;

	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Incorrect number of arguments for plane.");
	pl = malloc(sizeof(t_plane));
	if (!pl)
		erorr(scene, NULL, "Error: allocation failed.\n");
	if (!parse_vector(index_split(line, 1), &pl->point))
		erorr(scene, pl, "Error: Invalid vector format for plane point.");
	if (!parse_vector(index_split(line, 2), &pl->normal))
		erorr(scene, pl, "Error: Invalid vector format for plane normal.");
	validate_normalized_vector(pl->normal, scene);
	pl->normal = vec_normalize(pl->normal);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, pl, "Error: allocation failed.\n");
	new_obj->type = PLANE;
	new_obj->shape_data = pl;
	if (!parse_color(index_split(line, 3), &new_obj->color))
		erorr(scene, (free(new_obj), pl), \
		"Error: Invalid color format for plane.");
	validate_color(new_obj->color, scene);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}
