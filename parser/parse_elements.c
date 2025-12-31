#include "../include/minirt.h"

void	parse_ambient(t_scene *scene, char *line)
{
	ft_split_inplace(line, ' ');
	scene->ambient_light.ratio = ft_atof(index_split(line, 1));
	parse_vector(index_split(line, 2), &scene->ambient_light.color);
	scene->ambient_light.is_set = true;
}

void	parse_camera(t_scene *scene, char *line)
{
	ft_split_inplace(line, ' ');
	parse_vector(index_split(line, 1), &scene->camera.center);
	parse_vector(index_split(line, 2), &scene->camera.normal);
	scene->camera.normal = vec_normalize(scene->camera.normal);
	scene->camera.fov = ft_atof(index_split(line, 3));
	scene->camera.is_set = true;
}

void	parse_light(t_scene *scene, char *line)
{
	t_light	*nlight;

	ft_split_inplace(line, ' ');
	nlight = malloc(sizeof(t_light));
	if (!nlight)
		erorr(scene, NULL, "Error: Allocation failed.\n");
	add_light(scene, nlight);
	parse_vector(index_split(line, 1), &nlight->center);
	nlight->ratio = ft_atof(index_split(line, 2));
	parse_vector(index_split(line, 3), &nlight->color);
}
