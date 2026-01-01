#include "../include/minirt.h"

void	validate_reflectivity(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " reflectivity invalid.");
	}
	if (!is_valid_float(str) || !check_range(ft_atof(str), 0.0, 1.0))
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " reflectivity invalid.");
	}
}

void	validate_checker_color(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " checker color invalid.");
	}
	if (!check_color_fmt(str))
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " checker color invalid.");
	}
}

void	validate_texture(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " texture invalid.");
	}
	if (!has_extension(str, ".ppm"))
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " texture invalid.");
	}
}

void	validate_bumpmap(char *str, t_scene *scene, const char *object)
{
	if (!str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " bump map invalid.");
	}
	if (!has_extension(str, ".bump.ppm"))
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " bump map invalid.");
	}
}

void    validate_u_scale(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " u scale invalid.");
	}
	if (!is_valid_float(str) || ft_atof(str) <= 0)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " u scale invalid.");
	}
}

void    validate_v_scale(char *str, t_scene *scene, const char *object)
{
	if (!*str)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " v scale invalid.");
	}
	if (!is_valid_float(str) || ft_atof(str) <= 0)
	{
		write(2, "Error: ", 7);
		write(2, object, ft_strlen(object));
		erorr(scene, NULL, " v scale invalid.");
	}
}

/**
 * Check if the argument matches any of the valid argument prefixes
 * and call the corresponding validation function.
 * 
 * @note Will raise an error if the argument is invalid.
 */
bool    check_arg(char *arg, char **valid_args, t_scene *scene, const char *object)
{
	size_t	i;

	i = 0;
	while (valid_args[i])
	{
        printf("Checking arg: %s against valid prefix: %s\n", arg, valid_args[i]);
		if (ft_strncmp(arg, valid_args[i], ft_strlen(valid_args[i])) == 0)
		{
			if (ft_strncmp(arg, "r=", 2) == 0)
				validate_reflectivity(arg + 2, scene, object);
			else if (ft_strncmp(arg, "ch=", 3) == 0)
				validate_checker_color(arg + 3, scene, object);
			else if (ft_strncmp(arg, "tx=", 3) == 0)
				validate_texture(arg + 3, scene, object);
			else if (ft_strncmp(arg, "bump=", 5) == 0)
				validate_bumpmap(arg + 5, scene, object);
			else if (ft_strncmp(arg, "u=", 2) == 0)
				validate_u_scale(arg + 2, scene, object);
			else if (ft_strncmp(arg, "v=", 2) == 0)
				validate_v_scale(arg + 2, scene, object);
            return (true);
		}
		i++;
	}
	erorr(scene, NULL, "Error: Sphere extra argument invalid.");
    return (true);
}
