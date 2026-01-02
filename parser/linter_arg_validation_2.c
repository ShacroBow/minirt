/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linter_arg_validation_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:34 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:35 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

void	validate_reflectivity(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " reflectivity invalid.");
	if (!is_valid_float(str) || !check_range(ft_atof(str), 0.0, 1.0))
		erorrf(scene, NULL, obj_name, " reflectivity invalid.");
}

void	validate_checker_color(char *str, t_scene *scene, const char *obj_name)
{
	if (!*str)
		erorrf(scene, NULL, obj_name, " checker color invalid.");
	if (!check_color_fmt(str))
		erorrf(scene, NULL, obj_name, " checker color invalid.");
}
