#include "../minirt.h"

void	parse_ambient(t_scene *scene, char **tokens)
{
	if (scene->ambient_light.is_set)
		exit_error("Error: Ambient light can only be declared once.");
	if (count_tokens(tokens)!= 3)
		exit_error("Error: Incorrect number of arguments for ambient light.");
	scene->ambient_light.ratio = ft_atof(tokens[1]);
	validate_ratio(scene->ambient_light.ratio);
	if (!parse_color(tokens[2], &scene->ambient_light.color))
		exit_error("Error: Invalid color format for ambient light.");
	validate_color(scene->ambient_light.color);
	scene->ambient_light.is_set = true;
}

void	parse_camera(t_scene *scene, char **tokens)
{
	if (scene->camera.is_set)
		exit_error("Error: Camera can only be declared once.");
	if (count_tokens(tokens)!= 4)
		exit_error("Error: Incorrect number of arguments for camera.");
	if (!parse_vector(tokens[1], &scene->camera.center))
		exit_error("Error: Invalid vector format for camera center.");
	if (!parse_vector(tokens[2], &scene->camera.normal))
		exit_error("Error: Invalid vector format for camera normal.");
	validate_normalized_vector(scene->camera.normal);
	scene->camera.normal = vec_normalize(scene->camera.normal);
	scene->camera.fov = ft_atof(tokens[3]);
	validate_fov(scene->camera.fov);
	scene->camera.is_set = true;
}

void	parse_light(t_scene *scene, char **tokens)
{
	t_light	*new_light;
	t_light	*current;

	if (count_tokens(tokens)!= 4)
		exit_error("Error: Incorrect number of arguments for light.");
	new_light = safe_malloc(sizeof(t_light));
	if (!parse_vector(tokens[1], &new_light->center))
		exit_error("Error: Invalid vector format for light center.");
	new_light->ratio = ft_atof(tokens[2]);
	validate_ratio(new_light->ratio);
	if (!parse_color(tokens[3], &new_light->color))
		exit_error("Error: Invalid color format for light.");
	validate_color(new_light->color);
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

void	parse_sphere(t_scene *scene, char **tokens)
{
	t_object	*new_obj;
	t_sphere	*sp;

	if (count_tokens(tokens)!= 4)
		exit_error("Error: Incorrect number of arguments for sphere.");
	sp = safe_malloc(sizeof(t_sphere));
	if (!parse_vector(tokens[1], &sp->center))
		exit_error("Error: Invalid vector format for sphere center.");
	sp->diameter = ft_atof(tokens[2]);
	if (sp->diameter <= 0)
		exit_error("Error: Sphere diameter must be positive.");
	new_obj = safe_malloc(sizeof(t_object));
	new_obj->type = SPHERE;
	new_obj->shape_data = sp;
	if (!parse_color(tokens[3], &new_obj->color))
		exit_error("Error: Invalid color format for sphere.");
	validate_color(new_obj->color);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}

void	parse_plane(t_scene *scene, char **tokens)
{
	t_object	*new_obj;
	t_plane		*pl;

	if (count_tokens(tokens)!= 4)
		exit_error("Error: Incorrect number of arguments for plane.");
	pl = safe_malloc(sizeof(t_plane));
	if (!parse_vector(tokens[1], &pl->point))
		exit_error("Error: Invalid vector format for plane point.");
	if (!parse_vector(tokens[2], &pl->normal))
		exit_error("Error: Invalid vector format for plane normal.");
	validate_normalized_vector(pl->normal);
	pl->normal = vec_normalize(pl->normal);
	new_obj = safe_malloc(sizeof(t_object));
	new_obj->type = PLANE;
	new_obj->shape_data = pl;
	if (!parse_color(tokens[3], &new_obj->color))
		exit_error("Error: Invalid color format for plane.");
	validate_color(new_obj->color);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}

void	parse_cylinder(t_scene *scene, char **tokens)
{
	t_object	*new_obj;
	t_cylinder	*cy;

	if (count_tokens(tokens)!= 6)
		exit_error("Error: Incorrect number of arguments for cylinder.");
	cy = safe_malloc(sizeof(t_cylinder));
	if (!parse_vector(tokens[1], &cy->center))
		exit_error("Error: Invalid vector format for cylinder center.");
	if (!parse_vector(tokens[2], &cy->normal))
		exit_error("Error: Invalid vector format for cylinder normal.");
	validate_normalized_vector(cy->normal);
	cy->normal = vec_normalize(cy->normal);
	cy->diameter = ft_atof(tokens[3]);
	cy->height = ft_atof(tokens[4]);
	if (cy->diameter <= 0 || cy->height <= 0)
		exit_error("Error: Cylinder diameter and height must be positive.");
	new_obj = safe_malloc(sizeof(t_object));
	new_obj->type = CYLINDER;
	new_obj->shape_data = cy;
	if (!parse_color(tokens[5], &new_obj->color))
		exit_error("Error: Invalid color format for cylinder.");
	validate_color(new_obj->color);
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}