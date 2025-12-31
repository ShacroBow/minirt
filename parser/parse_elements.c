
#include "../include/minirt.h"

void	parse_ambient(t_scene *scene, char *line)
{
	ft_split_inplace(line, ' ');
	scene->ambient_light.ratio = ft_atof(index_split(line, 1));
	parse_vector(index_split(line, 2), &scene->ambient_light.color);
	scene->ambient_light.is_set = true;
}

void	parse_camera(t_scene *scene, char *line)
{
	ft_split_inplace(line, ' ');
	parse_vector(index_split(line, 1), &scene->camera.center);
	parse_vector(index_split(line, 2), &scene->camera.normal);
	scene->camera.normal = vec_normalize(scene->camera.normal);
	scene->camera.fov = ft_atof(index_split(line, 3));
	scene->camera.is_set = true;
}

void	parse_light(t_scene *scene, char *line)
{
	t_light	*nlight;

	ft_split_inplace(line, ' ');
	nlight = malloc(sizeof(t_light));
	if (!nlight)
		erorr(scene, NULL, "Error: allocation failed.\n");
	add_light(scene, nlight);
	parse_vector(index_split(line, 1), &nlight->center);
	nlight->ratio = ft_atof(index_split(line, 2));
	parse_vector(index_split(line, 3), &nlight->color);
}

void	parse_sphere(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_sphere	*sp;
	int		count;

	count = ft_split_inplace(line, ' ');
	sp = malloc(sizeof(t_sphere));
	if (!sp)
		erorr(scene, NULL, "Error: allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, sp, "Error: allocation failed.\n");
	new_obj->type = SPHERE;
	new_obj->shape_data = sp;
	new_obj->has_texture = false;
	new_obj->texture = NULL;
	new_obj->has_bump = false;
	new_obj->bump = NULL;
	new_obj->bump_enabled = false;
	new_obj->uv_scale_u = 1.0;
	new_obj->uv_scale_v = 1.0;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 1), &sp->center);
	sp->diameter = ft_atof(index_split(line, 2));
	parse_vector(index_split(line, 3), &new_obj->color);
	if (count >= 5)
		new_obj->reflectivity = ft_atof(index_split(line, 4));
	else
		new_obj->reflectivity = 0.0;
	/* optional checker/texture/bump at index 5 and optional extra file at index 6 */
	new_obj->has_checkerboard = false;
	if (count >= 6)
	{
		if (check_color_fmt(index_split(line, 5)))
		{
			new_obj->has_checkerboard = true;
			parse_vector(index_split(line, 5), &new_obj->checker_color);
		}
		else if (check_ppm_filename(index_split(line, 5)))
		{
			if (has_extension(index_split(line, 5), ".bump.ppm"))
			{
				new_obj->bump = load_ppm(index_split(line, 5));
				if (!new_obj->bump)
					erorr(scene, NULL, "Error: Sphere bump texture invalid.");
				new_obj->has_bump = true;
				new_obj->bump_enabled = true;
			}
			else
			{
				new_obj->texture = load_ppm(index_split(line, 5));
				if (!new_obj->texture)
					erorr(scene, NULL, "Error: Sphere texture invalid.");
				new_obj->has_texture = true;
				new_obj->color = (t_color){255.0, 255.0, 255.0};
					/* set sensible default UV scales based on sphere size and texture */
					if (new_obj->texture && sp)
					{
						new_obj->uv_scale_u = compute_uv_scale(M_PI * sp->diameter, new_obj->texture->width);
						new_obj->uv_scale_v = compute_uv_scale(sp->diameter, new_obj->texture->height);
					}
			}
			/* optional per-object UV scales: index 7 and 8 (1-based) */
			if (count >= 8)
				new_obj->uv_scale_u = ft_atof(index_split(line, 7));
			if (count >= 9)
				new_obj->uv_scale_v = ft_atof(index_split(line, 8));
			/* optional per-object UV scales: index 7 and 8 (1-based) */
			if (count >= 8)
				new_obj->uv_scale_u = ft_atof(index_split(line, 7));
			if (count >= 9)
				new_obj->uv_scale_v = ft_atof(index_split(line, 8));
		}
		else
			erorr(scene, NULL, "Error: Sphere checker color or texture invalid.");
	}
	if (count >= 7)
	{
		/* second file: could be additional texture or bump */
		if (check_ppm_filename(index_split(line, 6)))
		{
			if (has_extension(index_split(line, 6), ".bump.ppm"))
			{
				new_obj->bump = load_ppm(index_split(line, 6));
				if (!new_obj->bump)
					erorr(scene, NULL, "Error: Sphere bump texture invalid.");
				new_obj->has_bump = true;
				new_obj->bump_enabled = true;
			}
			else
			{
				new_obj->texture = load_ppm(index_split(line, 6));
				if (!new_obj->texture)
					erorr(scene, NULL, "Error: Sphere texture invalid.");
				new_obj->has_texture = true;
				new_obj->color = (t_color){255.0, 255.0, 255.0};
					/* set sensible default UV scales for second texture if present */
					if (new_obj->texture && sp)
					{
						new_obj->uv_scale_u = compute_uv_scale(M_PI * sp->diameter, new_obj->texture->width);
						new_obj->uv_scale_v = compute_uv_scale(sp->diameter, new_obj->texture->height);
					}
			}
		}
		else
			erorr(scene, NULL, "Error: Sphere texture invalid.");
	}
}

void	parse_plane(t_scene *scene, char *line)
{
	t_object	*new_obj;
	t_plane		*pl;
	int		count;

	count = ft_split_inplace(line, ' ');
	pl = malloc(sizeof(t_plane));
	if (!pl)
		erorr(scene, NULL, "Error: allocation failed.\n");
	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		erorr(scene, pl, "Error: allocation failed.\n");
	new_obj->type = PLANE;
	new_obj->shape_data = pl;
	new_obj->has_texture = false;
	new_obj->texture = NULL;
	new_obj->has_bump = false;
	new_obj->bump = NULL;
	new_obj->bump_enabled = false;
	new_obj->uv_scale_u = 1.0;
	new_obj->uv_scale_v = 1.0;
	add_object(scene, new_obj);
	parse_vector(index_split(line, 1), &pl->point);
	parse_vector(index_split(line, 2), &pl->normal);
	parse_vector(index_split(line, 3), &new_obj->color);
	if (count >= 5)
		new_obj->reflectivity = ft_atof(index_split(line, 4));
	else
		new_obj->reflectivity = 0.0;
	/* optional checker/texture/bump at index 5 and optional extra file at index 6 */
	new_obj->has_checkerboard = false;
	if (count >= 6)
	{
		if (check_color_fmt(index_split(line, 5)))
		{
			new_obj->has_checkerboard = true;
			parse_vector(index_split(line, 5), &new_obj->checker_color);
		}
		else if (check_ppm_filename(index_split(line, 5)))
		{
			if (has_extension(index_split(line, 5), ".bump.ppm"))
			{
				new_obj->bump = load_ppm(index_split(line, 5));
				if (!new_obj->bump)
					erorr(scene, NULL, "Error: Plane bump texture invalid.");
				new_obj->has_bump = true;
				new_obj->bump_enabled = true;
			}
			else
			{
				new_obj->texture = load_ppm(index_split(line, 5));
				if (!new_obj->texture)
					erorr(scene, NULL, "Error: Plane texture invalid.");
				new_obj->has_texture = true;
				new_obj->color = (t_color){255.0, 255.0, 255.0};
					/* sensible defaults: planes are unit-scaled, but scale by texture resolution */
					if (new_obj->texture)
					{
						new_obj->uv_scale_u = compute_uv_scale(1.0, new_obj->texture->width);
						new_obj->uv_scale_v = compute_uv_scale(1.0, new_obj->texture->height);
					}
			}
		}
		else
			erorr(scene, NULL, "Error: Plane checker color or texture invalid.");
	}
	if (count >= 7)
	{
		if (check_ppm_filename(index_split(line, 6)))
		{
			if (has_extension(index_split(line, 6), ".bump.ppm"))
			{
				new_obj->bump = load_ppm(index_split(line, 6));
				if (!new_obj->bump)
					erorr(scene, NULL, "Error: Plane bump texture invalid.");
				new_obj->has_bump = true;
				new_obj->bump_enabled = true;
			}
			else
			{
				new_obj->texture = load_ppm(index_split(line, 6));
				if (!new_obj->texture)
					erorr(scene, NULL, "Error: Plane texture invalid.");
				new_obj->has_texture = true;
				new_obj->color = (t_color){255.0, 255.0, 255.0};
					if (new_obj->texture)
					{
						new_obj->uv_scale_u = compute_uv_scale(1.0, new_obj->texture->width);
						new_obj->uv_scale_v = compute_uv_scale(1.0, new_obj->texture->height);
					}
			}
		}
		else
			erorr(scene, NULL, "Error: Plane texture invalid.");
	}
}
