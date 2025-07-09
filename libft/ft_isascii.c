/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:50:52 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/19 15:23:59 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	else
		return (0);
}
/*
#include <stdio.h>
#include <ctype.h>

int	main(void)
{

	printf("yay %i\n", isascii(127));
	printf("yay %i", ft_isascii(127));
	return (0);
} */
