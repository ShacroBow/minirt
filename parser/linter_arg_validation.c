/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linter_arg_validation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:36 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:37 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			if (ft_strncmp(arg, "r=", 2) == 0)
				validate_reflectivity(arg + 2, scene, obj_name);
			else if (ft_strncmp(arg, "ch=", 3) == 0)
				validate_checker_color(arg + 3, scene, obj_name);
			else if (ft_strncmp(arg, "tx=", 3) == 0)
				validate_texture(arg + 3, scene, obj_name);
			else if (ft_strncmp(arg, "bump=", 5) == 0)
				validate_bumpmap(arg + 5, scene, obj_name);
			else if (ft_strncmp(arg, "u=", 2) == 0)
				validate_u_scale(arg + 2, scene, obj_name);
			else if (ft_strncmp(arg, "v=", 2) == 0)
				validate_v_scale(arg + 2, scene, obj_name);
			return ;
		}
	}
	erorrf(scene, NULL, obj_name, " argument invalid.");
}
