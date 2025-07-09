/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:49:53 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/19 20:01:19 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if ((c >= 65 && c <= 90)
		|| (c >= 97 && c <= 122)
		|| (c >= 48 && c <= 57))
	{
		return (8);
	}
	else
		return (0);
}
/* 
#include <stdio.h>
#include <ctype.h>

int	main(void)
{

	printf("yay %i\n", isalnum('q'));
	printf("yay %i", ft_isalnum('q'));
	return (0);
}
 */