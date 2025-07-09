/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:18:57 by kmashkoo          #+#    #+#             */
/*   Updated: 2025/04/29 20:34:08 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *nptr, int *len)
{
	int			i;
	long int	sign;
	long int	res;

	if (nptr == NULL || nptr[0] == '\0')
		return (0);
	i = 0;
	*len = 0;
	sign = 1;
	while ((nptr[i] == '-' || nptr[i] == '+') || ft_isspace(nptr[i]))
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	res = 0;
	while (nptr[i] == '0')
		i++;
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		res = (res * 10) + (nptr[i] - '0');
		i++;
		*len += 1;
	}
	return (res * sign);
}

/* #include <stdlib.h>
#include <stdio.h>
int main(void)
{
	const char	*str = "    -123000004ab567";
	int			len;
	long		custom	= ft_atol(str, &len);
	long		orig	= atol(str);

	printf("ft_atol: %ld (digits parsed: %d)\n", custom, len);
	printf("   atol: %ld\n", orig);
	return 0;
} */