/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:57 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:58 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

void	add_light(t_scene *scene, t_light *new_light)
{
	t_light	*current;

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

void	add_object(t_scene *scene, t_object *new_obj)
{
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}
