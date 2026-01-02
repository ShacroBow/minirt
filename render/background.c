/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:59 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:17:00 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

t_color	bg_color_from_dir(const t_scene *scene, const t_vec3 *dir)
{
	t_vec3	unit_dir;
	double	proj;
	double	t;
	t_color	white;
	t_color	blue;

	if (ENABLE_BG)
	{
		unit_dir = vec_normalize(*dir);
		proj = vec_dot(unit_dir, scene->camera.up_dir);
		t = 0.5 * (proj + 1.0);
		white = (t_color){255.0, 255.0, 255.0};
		blue = (t_color){127.5, 178.5, 255.0};
		return (color_add(color_scale(white, 1.0 - t), color_scale(blue, t)));
	}
	(void)scene;
	(void)dir;
	return ((t_color){0, 0, 0});
}
