/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lint_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:32 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:33 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

static void	lint_sphere_extra(char *line, t_scene *scene, size_t count)
{
	size_t	i;
	char	*arg;
	char	*valid_args[7];

	i = 4;
	valid_args[0] = "r=";
	valid_args[1] = "ch=";
	valid_args[2] = "tx=";
	valid_args[3] = "bump=";
	valid_args[4] = "u=";
	valid_args[5] = "v=";
	valid_args[6] = NULL;
	while (i < count)
	{
		arg = index_split(line, i);
		check_arg(arg, valid_args, scene, "Sphere");
		i++;
	}
}

void	lint_sphere(char *line, t_scene *scene)
{
	size_t	count;

	count = ft_split_inplace(line, ' ');
	if (count < 4 || count > 10)
		erorr(scene, NULL, "Error: Sphere args count.");
	if (!check_vector_fmt(index_split(line, 1)))
		erorr(scene, NULL, "Error: Sphere center invalid.");
	if (!is_valid_float(index_split(line, 2)) || \
		ft_atof(index_split(line, 2)) <= 0)
		erorr(scene, NULL, "Error: Sphere diameter invalid.");
	if (!check_color_fmt(index_split(line, 3)))
		erorr(scene, NULL, "Error: Sphere color invalid.");
	if (count >= 5)
		lint_sphere_extra(line, scene, count);
}
