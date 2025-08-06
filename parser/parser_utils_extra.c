#include "../minirt.h"

/*
** parse a 3D vector from a string
*/
bool    parse_vector(char *str, t_vec3 *vec)
{
    char **parts = ft_split(str, ',');
    if (!parts || count_tokens(parts) != 3)
    {
        free_tokens(parts);
        return false;
    }
    vec->x = ft_atof(parts[0]);
    vec->y = ft_atof(parts[1]);
    vec->z = ft_atof(parts[2]);
    free_tokens(parts);
    return true;
}

/*
** parse a color vector from a string
*/
bool    parse_color(char *str, t_color *color)
{
    char **parts = ft_split(str, ',');
    if (!parts || count_tokens(parts) != 3)
    {
        free_tokens(parts);
        return false;
    }
    color->x = ft_atof(parts[0]);
    color->y = ft_atof(parts[1]);
    color->z = ft_atof(parts[2]);
    free_tokens(parts);
    return true;
}

/*
** parse ratio (e.g. ambient or light ratio) from parts[0]
*/
double  parse_ratio(char *str)
{
    double  ratio;

    ratio = ft_atof(str);
    if (ratio < 0.0 || ratio > 1.0)
        exit_error("Error: ratio must be between 0.0 and 1.0");
    return (ratio);
}

/*
** parse Field of View from parts[0]
*/
int     parse_fov(char *str)
{
    int fov;

    fov = (int)ft_atof(str);
    if (fov < 0 || fov > 180)
        exit_error("Error: FOV must be between 0 and 180");
    return (fov);
}
