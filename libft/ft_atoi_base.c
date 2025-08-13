/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:17:20 by kmashkoo          #+#    #+#             */
/*   Updated: 2025/08/09 19:45:00 by kha              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isvalid(int ch, int baselen)
{
	char	*lcbase;
	char	*ucbase;
	int		i;

	i = 0;
	lcbase = "0123456789abcdef";
	ucbase = "0123456789ABCDEF";
	while (i < baselen && baselen < 17)
	{
		if (ch == lcbase[i] || ch == ucbase[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi_base(const char *str, int base)
{
	int	res;
	int	i;
	int	j;

	res = 0;
	i = 0;
	j = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			j *= -1;
	while (str[i] && isvalid(str[i], base))
	{
		res *= base;
		if (str[i] >= '0' && str[i] <= '9')
			res += str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			res += str[i] - 'a' + 10;
		else if (str[i] >= 'A' && str[i] <= 'F')
			res += str[i] - 'A' + 10;
		i++;
	}
	return (res * j);
}


/* #include <stdio.h>
int	main(int argc, char **argv)
{
	int	base;
	int	value;

	if (argc != 3)
	{
		printf("Usage: %s <number_string> <base>\n", argv[0]);
		return (1);
	}
	base = ft_atoi_base(argv[2], 10);
	if (base < 2 || base > 16)
	{
		printf("Error: base must be 2..16 (got %d)\n", base);
		return (1);
	}
	value = ft_atoi_base(argv[1], base);
	printf("Input: %s | Base: %d | Result: %d\n", argv[1], base, value);
	printf("Checks: 42(dec)= %d | 2A(hex)= %d | 101010(bin)= %d\n",
		ft_atoi_base("42", 10), ft_atoi_base("2A", 16), ft_atoi_base("101010", 2));
	return (0);
} */