/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linter_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:37 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:38 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

void	lint_ambient(char *line, t_scene *scene)
{
	if (ft_split_inplace(line, ' ') != 3)
		erorr(scene, NULL, "Error: Ambient light args count.");
	if (!is_valid_float(index_split(line, 1)) || \
		!check_range(ft_atof(index_split(line, 1)), 0.0, 1.0))
		erorr(scene, NULL, "Error: Ambient light ratio invalid.");
	if (!check_color_fmt(index_split(line, 2)))
		erorr(scene, NULL, "Error: Ambient light color invalid.");
}

void	lint_camera(char *line, t_scene *scene)
{
	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Camera args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Camera center invalid.");
	if (!check_normalized(index_split(line, 2)))
		erorr(scene, NULL, "Error: Camera normal invalid.");
	if (!is_valid_float(index_split(line, 3)) || \
		!check_range(ft_atof(index_split(line, 3)), 0, 180))
		erorr(scene, NULL, "Error: Camera FOV invalid.");
}

void	lint_light(char *line, t_scene *scene)
{
	if (ft_split_inplace(line, ' ') != 4)
		erorr(scene, NULL, "Error: Light args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Light center invalid.");
	if (!is_valid_float(index_split(line, 2)) || \
		!check_range(ft_atof(index_split(line, 2)), 0.0, 1.0))
		erorr(scene, NULL, "Error: Light ratio invalid.");
	if (!check_color_fmt(index_split(line, 3)))
		erorr(scene, NULL, "Error: Light color invalid.");
}
