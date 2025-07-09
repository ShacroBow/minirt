/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:57:17 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/04 16:59:43 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if ((c >= 32 && c <= 126))
	{
		return (16384);
	}
	else
		return (0);
}

/* 
#include <stdio.h>
#include <ctype.h>

int	main(void)
{

	printf("yay %i\n", isprint(126));
	printf("yay %i", ft_isprint(126));
	return (0);
} */
