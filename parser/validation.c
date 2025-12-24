#include "../include/minirt.h"

void	validate_color(t_color color, t_scene *scene)
{
	if (color.x < 0 || color.x > 255 || \
		color.y < 0 || color.y > 255 || \
		color.z < 0 || color.z > 255)
		(free(scene), exit_error("Color values must be in [0,255]"));
}

void	validate_normalized_vector(t_vec3 vec, t_scene *scene)
{
	if (vec.x < -1 || vec.x > 1 || \
		vec.y < -1 || vec.y > 1 || \
		vec.z < -1 || vec.z > 1)
		(free(scene), exit_error("Invalid normalized vector"));
	if (vec_lensqrt(vec) < EPSILON)
		(free(scene), exit_error("Error: Normal vector cannot be zero."));
}

void	validate_ratio(double ratio, t_scene *scene)
{
	if (ratio < 0.0 || ratio > 1.0)
		(erorr(scene, NULL, "Invalid ratio (must be between 0.0 and 1.0)"));
}

void	validate_fov(double fov, t_scene *scene)
{
	if (fov < 0 || fov > 180)
		(erorr(scene, NULL, "Error: FOV must be between 0 and 180."));
}

void	validate_scene(t_scene *scene)
{
	if (!scene->ambient_light.is_set)
		(erorr(scene, NULL, \
			"Error: Ambient light is not defined in the scene."));
	if (!scene->camera.is_set)
		(erorr(scene, NULL, "Error: Camera is not defined in the scene."));
}
