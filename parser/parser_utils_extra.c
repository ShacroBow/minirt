#include "../minirt.h"

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

double	ft_atof(const char *str)
{
	double	res;
	double	power;
	int		sign;

	res = 0.0;
	power = 1.0;
	sign = 1;
	while (*str == ' ' |

| (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' |

| *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (is_digit(*str))
		res = res * 10.0 + (*str++ - '0');
	if (*str == '.')
	{
		str++;
		while (is_digit(*str))
		{
			res = res * 10.0 + (*str++ - '0');
			power *= 10.0;
		}
	}
	return (sign * res / power);
}

bool	parse_vector(char *str, t_vec3 *vec)
{
	char	**parts;

	parts = ft_split(str, ',');
	if (count_tokens(parts)!= 3)
	{
		free_tokens(parts);
		return (false);
	}
	vec->x = ft_atof(parts);
	vec->y = ft_atof(parts[1]);
	vec->z = ft_atof(parts[2]);
	free_tokens(parts);
	return (true);
}

bool	parse_color(char *str, t_color *color)
{
	char	**parts;

	parts = ft_split(str, ',');
	if (count_tokens(parts)!= 3)
	{
		free_tokens(parts);
		return (false);
	}
	color->x = ft_atof(parts);
	color->y = ft_atof(parts[1]);
	color->z = ft_atof(parts[2]);
	free_tokens(parts);
	return (true);
}