/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:39:57 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:05:37 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if ((c >= 48 && c <= 57))
	{
		return (2048);
	}
	else
		return (0);
}
/*
#include <stdio.h>
#include <ctype.h>

int	main(void)
{

	printf("yay %i\n", isdigit('4'));
	printf("nay %i", ft_isdigit('4'));
	return (0);
} */