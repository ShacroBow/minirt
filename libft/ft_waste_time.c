/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_waste_time.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:14:32 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/12/16 22:51:04 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned long int	ft_waste_time(unsigned int iterations)
{
	unsigned long int	i;
	unsigned long int	j;
	unsigned long int	sum;
	unsigned long int	secondish;

	i = 0;
	sum = 0;
	secondish = (iterations * 700000);
	while (i < secondish)
	{
		j = 0;
		while (j < 1000)
		{
			sum++;
			j++;
		}
		sum++;
		i++;
	}
	return (sum);
}
/* 
#include <stdio.h>
int main()
{
	unsigned long long sum = ft_waste_time(2);
	printf("sum:%llu\n", sum);
	return 0;
} */