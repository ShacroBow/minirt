/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_waste_time.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:14:32 by kmashkoo          #+#    #+#             */
/*   Updated: 2025/12/24 19:42:54 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_waste_time(unsigned int iterations)
{
	size_t	i;
	size_t	j;
	size_t	sum;
	size_t	secondish;

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
