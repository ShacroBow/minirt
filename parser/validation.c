#include "../minirt.h"

void	validate_color(t_color color)
{
	if (color.x < 0 || color.x > 255 ||
		color.y < 0 || color.y > 255 ||
		color.z < 0 || color.z > 255)
		exit_error("Color values must be in [0,255]");
}

void	validate_normalized_vector(t_vec3 vec)
{
	if (vec.x < -1 || vec.x > 1 ||
		vec.y < -1 || vec.y > 1 ||
		vec.z < -1 || vec.z > 1)
		exit_error("Invalid normalized vector");
	if (vec_length_squared(vec) < EPSILON)
		exit_error("Error: Normal vector cannot be zero.");
}

void	validate_ratio(double ratio)
{
	if (ratio < 0.0 || ratio > 1.0)
		exit_error("Invalid ratio (must be between 0.0 and 1.0)");
}

void	validate_fov(double fov)
{
	if (fov < 0 || fov > 180)
		exit_error("Error: FOV must be between 0 and 180.");
}

void	validate_scene(t_scene *scene)
{
	if (!scene->ambient_light.is_set)
		exit_error("Error: Ambient light is not defined in the scene.");
	if (!scene->camera.is_set)
		exit_error("Error: Camera is not defined in the scene.");
}