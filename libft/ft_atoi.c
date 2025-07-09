/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 18:37:02 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:16:58 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_atoicalc_atoi(char *ptr, int i)
{
	int	k;

	k = 0;
	while (ptr[i] >= 48 && ptr[i] <= 57)
	{
		k *= 10;
		k += (ptr[i] - 48);
		i++;
	}
	return (k);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		j;
	int		k;
	char	*ptr;

	i = 0;
	j = 1;
	ptr = (char *)nptr;
	while ((ptr[i] >= 9 && ptr[i] <= 13) || ptr[i] == 32)
		i++;
	if (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			j = -1;
		else
			j = 1;
		i++;
	}
	k = ft_atoicalc_atoi(ptr, i);
	return (k * j);
}
/* 
#include <stdlib.h>
#include <stdio.h>
int main (void)
{
	char	*str = "    -1234ab567";
	
	printf("my script:%d.\n", ft_atoi(str));

	printf("my original:%d.\n", atoi(str));

	return (0);	
} */