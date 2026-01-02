/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:53 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:54 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

static bool	split3_in_place(char *str, char **a, char **b, char **c)
{
	char	*p1;
	char	*p2;

	if (!str)
		return (false);
	p1 = ft_strchr(str, ',');
	if (!p1)
		return (false);
	*p1 = '\0';
	*b = p1 + 1;
	p2 = ft_strchr(*b, ',');
	if (!p2)
		return (false);
	*p2 = '\0';
	*c = p2 + 1;
	if (ft_strchr(*c, ','))
		return (false);
	*a = str;
	return (true);
}

static bool	ft_valid_3str(char *str)
{
	int	commas;
	int	i;

	if (!str)
		return (false);
	commas = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			commas++;
		else if (!(ft_isspace(str[i]) || ft_isdigit(str[i])
				|| str[i] == '.' || str[i] == '+' || str[i] == '-'))
			return (false);
		i++;
	}
	return (commas == 2);
}

bool	parse_vector(char *str, t_vec3 *vec)
{
	char	*a;
	char	*b;
	char	*c;
	char	*tmp_str;

	if (!ft_valid_3str(str))
		return (false);
	tmp_str = ft_strdup(str);
	if (!tmp_str)
		return ((void) !write(2, "Error: Allocation failed\n", 25), false);
	if (!split3_in_place(tmp_str, &a, &b, &c))
	{
		free(tmp_str);
		return (false);
	}
	if (!is_valid_float(a) || !is_valid_float(b) || !is_valid_float(c))
	{
		free(tmp_str);
		return (false);
	}
	vec->x = ft_atof(a);
	vec->y = ft_atof(b);
	vec->z = ft_atof(c);
	free(tmp_str);
	return (true);
}

bool	is_ignorable(const char *s)
{
	int	i;

	if (!s)
		return (true);
	i = 0;
	while (s[i] && ft_isspace((unsigned char)s[i]))
		i++;
	return (s[i] == '\0' || s[i] == '#');
}

void	validate_object_extra_args(t_object *obj, t_scene *scene,
			const char *name)
{
	if (obj->has_checkerboard && obj->has_texture)
		obj->checker_enabled = false;
	if (obj->has_bump && !obj->has_texture)
		erorrf(scene, NULL, name, " bump map without texture.");
}
