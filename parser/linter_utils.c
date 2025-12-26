#include "../include/minirt.h"

bool	is_valid_float(char *str)
{
	int	i;
	int	dot_count;

	i = 0;
	dot_count = 0;
	if (!str)
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] == '.')
			dot_count++;
		else if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (dot_count <= 1);
}

bool	check_range(double val, double min, double max)
{
	return (val >= min && val <= max);
}

bool	check_color_fmt(char *str)
{
	t_color	c;

	if (!parse_vector(str, &c))
		return (false);
	return (check_range(c.x, 0, 255) && \
			check_range(c.y, 0, 255) && \
			check_range(c.z, 0, 255));
}

bool	check_vector_fmt(char *str)
{
	t_vec3	v;

	return (parse_vector(str, &v));
}

bool	check_normalized(char *str)
{
	t_vec3	v;

	if (!parse_vector(str, &v))
		return (false);
	if (v.x < -1.0001 || v.x > 1.0001 || \
		v.y < -1.0001 || v.y > 1.0001 || \
		v.z < -1.0001 || v.z > 1.0001)
		return (false);
	if (vec_lensqrt(v) < EPSILON)
		return (false);
	return (true);
}
