#include "../include/minirt.h"

int	is_ws(int c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v'
		|| c == '\f');
}

int	skip_ws_and_comments(const char *s, int i)
{
	while (s[i])
	{
		while (s[i] && is_ws(s[i]))
			i++;
		if (s[i] != '#')
			break ;
		while (s[i] && s[i] != '\n')
			i++;
	}
	return (i);
}

int	parse_int(const char *s, int *i, int *out)
{
	int	v;

	v = 0;
	*i = skip_ws_and_comments(s, *i);
	if (!ft_isdigit(s[*i]))
		return (0);
	while (ft_isdigit(s[*i]))
	{
		v = v * 10 + (s[*i] - '0');
		(*i)++;
	}
	*out = v;
	return (1);
}

int	ppm_header(const char *s, int *i, int *w, int *h)
{
	int	max;

	*i = skip_ws_and_comments(s, *i);
	if (ft_strncmp(s + *i, "P6", 2) != 0)
		return (0);
	*i += 2;
	if (!parse_int(s, i, w))
		return (0);
	if (!parse_int(s, i, h))
		return (0);
	if (!parse_int(s, i, &max))
		return (0);
	if (max <= 0 || max > 255)
		return (0);
	*i = skip_ws_and_comments(s, *i);
	return (1);
}

double	clamp01(double x)
{
	if (x < 0.0)
		return (0.0);
	if (x > 1.0)
		return (1.0);
	return (x);
}
