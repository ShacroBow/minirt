#include "../minirt.h"

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

bool	parse_vector(char *str, t_vec3 *vec)
{
	char	*a;
	char	*b;
	char	*c;

	if (!split3_in_place(str, &a, &b, &c))
		return (false);
	vec->x = ft_atof(a);
	vec->y = ft_atof(b);
	vec->z = ft_atof(c);
	return (true);
}

bool	parse_color(char *str, t_color *color)
{
	char	*a;
	char	*b;
	char	*c;

	if (!split3_in_place(str, &a, &b, &c))
		return (false);
	color->x = ft_atof(a);
	color->y = ft_atof(b);
	color->z = ft_atof(c);
	return (true);
}
