#include "../include/minirt.h"

void	parse_ambient(t_scene *scene, char *line)
{
	ft_split_inplace(line, ' ');
	scene->ambient_light.ratio = ft_atof(index_split(line, 1));
	if (!parse_vector(index_split(line, 2), &scene->ambient_light.color))
		erorr(scene, NULL, "Error parsing ambient light color");
	scene->ambient_light.is_set = true;
}

void	parse_camera(t_scene *scene, char *line)
{
	ft_split_inplace(line, ' ');
	if (!parse_vector(index_split(line, 1), &scene->camera.center)
		|| !parse_vector(index_split(line, 2), &scene->camera.normal))
		erorr(scene, NULL, "Error parsing camera vectors");
	scene->camera.normal = vec_normalize(scene->camera.normal);
	scene->camera.fov = ft_atof(index_split(line, 3));
	scene->camera.is_set = true;
}

void	init_light_obj(t_scene *scene, t_object *obj, t_light *nlight)
{
	obj->shape_data = nlight;
	obj->type = LIGHT;
	obj->name = ft_strdup("Light");
	if (!obj->name)
		erorr(scene, NULL, "Erorr: Allocation failed.");
}

void	parse_light(t_scene *scene, char *line)
{
	t_light		*nlight;
	t_object	*obj;

	ft_split_inplace(line, ' ');
	nlight = malloc(sizeof(t_light));
	if (!nlight)
		erorr(scene, NULL, "Error: Allocation failed.");
	obj = malloc(sizeof(t_object));
	if (!obj)
		erorr(scene, nlight, "Error: Allocation failed.");
	ft_bzero(obj, sizeof(t_object));
	add_light(scene, nlight);
	add_object(scene, obj);
	init_light_obj(scene, obj, nlight);
	if (!parse_vector(index_split(line, 1), &nlight->center)
		|| !parse_vector(index_split(line, 3), &nlight->color))
		erorr(scene, NULL, "Error parsing light");
	nlight->ratio = ft_atof(index_split(line, 2));
}
