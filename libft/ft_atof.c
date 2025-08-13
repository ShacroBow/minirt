#include "libft.h"

static void	init_parse(const char *s, int *i, double *sign)
{
	while (ft_isspace(s[*i]))
		(*i)++;
	while (s[*i] == '-' || s[*i] == '+')
	{
		if (s[*i] == '-')
			*sign = -*sign;
		(*i)++;
	}
}

static void	parse_fraction(const char *s, int *i, int *digits, double *res)
{
	double	frac;
	double	denom;

	if (s[*i] != '.')
		return ;
	(*i)++;
	frac = 0.0;
	denom = 1.0;
	while (ft_isdigit(s[*i]))
	{
		frac = frac * 10.0 + (s[*i] - '0');
		denom *= 10.0;
		(*i)++;
		(*digits)++;
	}
	*res += frac / denom;
}

static double	parse_number(const char *s, int *i, int *digits)
{
	double	res;

	res = 0.0;
	while (s[*i] == '0')
		(*i)++;
	while (ft_isdigit(s[*i]))
	{
		res = res * 10.0 + (s[*i] - '0');
		(*i)++;
		(*digits)++;
	}
	parse_fraction(s, i, digits, &res);
	return (res);
}

static double	ft_atof_len(const char *s, int *digits)
{
	int		i;
	double	sign;
	double	val;

	if (!s || !digits)
		return (0.0);
	i = 0;
	*digits = 0;
	sign = 1.0;
	init_parse(s, &i, &sign);
	val = parse_number(s, &i, digits);
	return (val * sign);
}

double	ft_atof(const char *str)
{
	int		len;
	double	val;

	if (!str)
		return (0.0);
	len = 0;
	val = ft_atof_len(str, &len);
	if (len == 0)
		return (0.0);
	return (val);
}

/* 
#include <stdio.h>
int main(void)
{
	int d; double v;
	v = ft_atof("  -0012.340x");
	printf("val=%f len=%d\n", v, ft_atof_len("  -0012.340x", &d));
	return 0;
} */

