#include "../include/minirt.h"

static t_cone	*parse_cone_struct(t_scene *scene, char *line)
{
	t_cone	*co;

	ft_split_inplace(line, ' ');
	co = malloc(sizeof(t_cone));
	if (!co)
		erorr(scene, NULL, "Error: Allocation failed.\n");
	parse_vector(index_split(line, 1), &co->center);
	parse_vector(index_split(line, 2), &co->center_dir);
	co->center_dir = vec_normalize(co->center_dir);
	co->diameter = ft_atof(index_split(line, 3));
	co->height = ft_atof(index_split(line, 4));
	co->apex = vec_add(co->center, \
		vec_mult(co->center_dir, co->height / 2));
	co->center = vec_sub(co->center, \
		vec_mult(co->center_dir, co->height / 2));
	return (co);
}

static void	parse_cone_texture(t_object *new_obj, char *line, t_scene *scene)
{
	new_obj->texture = load_ppm(index_split(line, 8));
	if (!new_obj->texture)
		erorr(scene, NULL, "Error: Cone texture invalid.");
	new_obj->has_texture = true;
}

static void	parse_cone_checker(t_object *new_obj, char *line, t_scene *scene)
{
	if (check_color_fmt(index_split(line, 7)))
	{
		new_obj->has_checkerboard = true;
		parse_vector(index_split(line, 7), &new_obj->checker_color);
	}
	else if (has_extension(index_split(line, 7), ".ppm"))
	{
		new_obj->texture = load_ppm(index_split(line, 7));
		if (!new_obj->texture)
			erorr(scene, NULL, "Error: Cone texture invalid.");
		new_obj->has_texture = true;
	}
	else
		erorr(scene, NULL, "Error: Cone checker color or texture invalid.");
}

static void	parse_cone_extra(t_object *new_obj, char *line, \
							int count, t_scene *scene)
{
	if (count == 8)
		parse_cone_checker(new_obj, line, scene);
	else
		new_obj->has_checkerboard = false;
	if (count == 9)
		parse_cone_texture(new_obj, line, scene);
}

void	parse_cone(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_cone		*co;
	int			count;

	count = ft_split_inplace(line, ' ');
	co = parse_cone_struct(scene, line);
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, co, "Error: Allocation failed.\n");
	new_obj->type = CONE;
	new_obj->shape_data = co;
	new_obj->has_texture = false;
	new_obj->texture = NULL;
	new_obj->has_bump = false;
	new_obj->bump = NULL;
	new_obj->bump_enabled = false;
	new_obj->scale_u = 1.0;
	new_obj->scale_v = 1.0;
	new_obj->has_checkerboard = false;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 5), &new_obj->color);
	if (count >= 7)
		new_obj->reflectivity = ft_atof(index_split(line, 6));
	else
		new_obj->reflectivity = 0.0;
	if (count >= 8)
	{
		if (check_color_fmt(index_split(line, 7)))
		{
			new_obj->has_checkerboard = true;
			parse_vector(index_split(line, 7), &new_obj->checker_color);
		}
		else if (has_extension(index_split(line, 7), ".ppm"))
		{
			if (has_extension(index_split(line, 7), ".bump.ppm"))
			{
				new_obj->bump = load_ppm(index_split(line, 7));
				if (!new_obj->bump)
					erorr(scene, NULL, "Error: Cone bump texture invalid.");
				new_obj->has_bump = true;
				new_obj->bump_enabled = true;
			}
			else
			{
				new_obj->texture = load_ppm(index_split(line, 7));
				if (!new_obj->texture)
					erorr(scene, NULL, "Error: Cone texture invalid.");
				new_obj->has_texture = true;
				new_obj->color = (t_color){255.0, 255.0, 255.0};
					/* set sensible default UV scales: u ~ base circumference, v ~ height */
					if (new_obj->texture && co)
					{
						new_obj->scale_u = compute_uv_scale(M_PI * co->diameter, new_obj->texture->width);
						new_obj->scale_v = compute_uv_scale(co->height, new_obj->texture->height);
					}
			}
		/* optional per-object UV scales: index 9 and 10 (1-based) */
		if (count >= 10)
			new_obj->scale_u = ft_atof(index_split(line, 9));
		if (count >= 11)
			new_obj->scale_v = ft_atof(index_split(line, 10));
		}
		else
			erorr(scene, NULL, "Error: Cone checker color or texture invalid.");
	}
	else
		new_obj->has_checkerboard = false;
	if (count == 9)
	{
		if (has_extension(index_split(line, 8), ".bump.ppm"))
		{
			new_obj->bump = load_ppm(index_split(line, 8));
			if (!new_obj->bump)
				erorr(scene, NULL, "Error: Cone bump texture invalid.");
			new_obj->has_bump = true;
			new_obj->bump_enabled = true;
		}
		else
		{
			new_obj->texture = load_ppm(index_split(line, 8));
			if (!new_obj->texture)
				erorr(scene, NULL, "Error: Cone texture invalid.");
			new_obj->has_texture = true;
			new_obj->color = (t_color){255.0, 255.0, 255.0};
				if (new_obj->texture && co)
				{
					new_obj->scale_u = compute_uv_scale(M_PI * co->diameter, new_obj->texture->width);
					new_obj->scale_v = compute_uv_scale(co->height, new_obj->texture->height);
				}
		}
	}
	parse_cone_extra(new_obj, line, count, scene);
}
