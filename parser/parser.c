/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:16:55 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/02 18:16:56 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minirt.h"

static void	parse_line(char *line, t_scene *scene)
{
	if (is_ignorable(line))
		return ;
	if (ft_strncmp(line, "A ", 2) == 0)
		parse_ambient(scene, line);
	else if (ft_strncmp(line, "C ", 2) == 0)
		parse_camera(scene, line);
	else if (ft_strncmp(line, "L ", 2) == 0)
		parse_light(scene, line);
	else if (ft_strncmp(line, "sp ", 3) == 0)
		parse_sphere(scene, line);
	else if (ft_strncmp(line, "pl ", 3) == 0)
		parse_plane(scene, line);
	else if (ft_strncmp(line, "cy ", 3) == 0)
		parse_cylinder(scene, line);
	else if (ft_strncmp(line, "co ", 3) == 0)
		parse_cone(scene, line);
	else
		erorr(scene, NULL, "Error: Invalid identifier in scene file.");
}

static void	parse_loop(char *line, size_t line_count, t_scene *scene)
{
	size_t	line_len;

	while (line_count--)
	{
		while (*line == 0)
			line += 1;
		line_len = ft_strlen(line);
		if (!is_ignorable(line))
			parse_line(line, scene);
		line += line_len;
	}
}

static void	read_scene_content(const char *filename, t_scene *scene)
{
	int		fd;

	scene->file_content = malloc(FILE_SIZE + 1);
	if (!scene->file_content)
		erorr(scene, NULL, "Error: Allocation failure.");
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		erorr(scene, NULL, "Error: Cannot open scene file.");
	read_file(fd, scene->file_content, scene);
}

void	parse_scene(const char *filename, t_scene **scene)
{
	size_t		line_count;

	*scene = malloc(sizeof(t_scene));
	if (!*scene)
		erorr(NULL, NULL, "Error: Allocation failure.");
	ft_bzero(*scene, sizeof(t_scene));
	read_scene_content(filename, *scene);
	line_count = ft_split_inplace((*scene)->file_content, '\n');
	lint_scene((*scene)->file_content, line_count, *scene);
	parse_loop((*scene)->file_content, line_count, *scene);
}
