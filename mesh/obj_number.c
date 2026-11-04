#include "../include/mesh.h"

static double	read_digits(char **p, double v, int *count)
{
	while (**p >= '0' && **p <= '9')
	{
		v = v * 10.0 + (**p - '0');
		(*p)++;
		(*count)++;
	}
	return (v);
}

static bool	read_exponent(char **p, int *exp)
{
	char	*q;
	int		sign;
	int		count;
	double	v;

	*exp = 0;
	if (**p != 'e' && **p != 'E')
		return (true);
	q = *p + 1;
	sign = 1;
	if (*q == '-' || *q == '+')
	{
		if (*q == '-')
			sign = -1;
		q++;
	}
	count = 0;
	v = read_digits(&q, 0.0, &count);
	if (count == 0)
		return (false);
	*exp = sign * (int)fmin(v, 9999.0);
	*p = q;
	return (true);
}

static bool	read_mantissa(char **p, double *v, int *frac)
{
	int	count;
	int	before;

	count = 0;
	*v = read_digits(p, 0.0, &count);
	*frac = 0;
	if (**p == '.')
	{
		(*p)++;
		before = count;
		*v = read_digits(p, *v, &count);
		*frac = count - before;
	}
	return (count > 0);
}

/**
 * Reads a decimal number like "-1.25", ".5" or "3.1e-05" at *s (leading
 * whitespace allowed) and moves *s past it. The number has to end at
 * whitespace or the end of the line.
 */
bool	parse_real(char **s, double *out)
{
	char	*p;
	double	sign;
	double	v;
	int		frac;
	int		exp;

	p = skip_ws(*s);
	sign = 1.0;
	if (*p == '-' || *p == '+')
	{
		if (*p == '-')
			sign = -1.0;
		p++;
	}
	if (!read_mantissa(&p, &v, &frac) || !read_exponent(&p, &exp))
		return (false);
	if (*p && !is_blank(*p))
		return (false);
	if (exp - frac < 0)
		*out = sign * v / pow(10.0, frac - exp);
	else
		*out = sign * v * pow(10.0, exp - frac);
	*s = p;
	return (isfinite(*out));
}

/**
 * Reads an optionally signed integer at *s (no leading whitespace) and moves
 * *s past it.
 */
bool	parse_index(char **s, long *out)
{
	char	*p;
	long	sign;
	long	v;

	p = *s;
	sign = 1;
	if (*p == '-' || *p == '+')
	{
		if (*p == '-')
			sign = -1;
		p++;
	}
	if (*p < '0' || *p > '9')
		return (false);
	v = 0;
	while (*p >= '0' && *p <= '9')
	{
		if (v < 1000000000000L)
			v = v * 10 + (*p - '0');
		p++;
	}
	*out = sign * v;
	*s = p;
	return (true);
}
