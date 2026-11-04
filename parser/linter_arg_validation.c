#include "../include/minirt.h"

void	validate_texture(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " texture invalid.");
	if (!has_extension(str, ".ppm"))
		erorrf(scene, NULL, obj_name, " texture invalid.");
}

void	validate_bumpmap(char *str, t_scene *scene, const char *obj_name)
{
	if (!str)
		erorrf(scene, NULL, obj_name, " bump map invalid.");
	if (!has_extension(str, ".bump.ppm"))
		erorrf(scene, NULL, obj_name, " bump map invalid.");
}

void	validate_u_scale(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " u scale invalid.");
	if (!is_valid_float(str) || ft_atof(str) <= 0)
		erorrf(scene, NULL, obj_name, " u scale invalid.");
}

void	validate_v_scale(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " v scale invalid.");
	if (!is_valid_float(str) || ft_atof(str) <= 0)
		erorrf(scene, NULL, obj_name, " v scale invalid.");
}

/**
 * Check if the argument matches any of the valid argument prefixes
 * and call the corresponding validation function.
 * 
 * @note Will raise an error if the argument is invalid.
 */
void	check_arg(char *arg, char **valid_args, t_scene *scene,
			const char *obj_name)
{
	ssize_t	i;

	i = -1;
	while (valid_args[++i])
	{
		if (ft_strncmp(arg, valid_args[i], ft_strlen(valid_args[i])) == 0)
		{
			validate_arg_value(arg, scene, obj_name);
			return ;
		}
	}
	erorrf(scene, NULL, obj_name, " argument invalid.");
}
