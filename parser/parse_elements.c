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
		erorr(scene, NULL, "Error: allocation failed.\n");
	add_light(scene, nlight);
	parse_vector(index_split(line, 1), &nlight->center);
	nlight->ratio = ft_atof(index_split(line, 2));
	parse_vector(index_split(line, 3), &nlight->color);
}

void	parse_sphere(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_sphere	*sp;
	int			count;

	count = ft_split_inplace(line, ' ');
	sp = malloc(sizeof(t_sphere));
	if (!sp)
		erorr(scene, NULL, "Error: allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, sp, "Error: allocation failed.\n");
	new_obj->type = SPHERE;
	new_obj->shape_data = sp;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 1), &sp->center);
	sp->diameter = ft_atof(index_split(line, 2));
	parse_vector(index_split(line, 3), &new_obj->color);
	if (count == 5)
		new_obj->reflectivity = ft_atof(index_split(line, 4));
	else
		new_obj->reflectivity = 0.0;
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane		*pl;
	int			count;

	count = ft_split_inplace(line, ' ');
	pl = malloc(sizeof(t_plane));
	if (!pl)
		erorr(scene, NULL, "Error: allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, pl, "Error: allocation failed.\n");
	new_obj->type = PLANE;
	new_obj->shape_data = pl;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 1), &pl->point);
	parse_vector(index_split(line, 2), &pl->normal);
	parse_vector(index_split(line, 3), &new_obj->color);
	if (count == 5)
		new_obj->reflectivity = ft_atof(index_split(line, 4));
	else
		new_obj->reflectivity = 0.0;
}
