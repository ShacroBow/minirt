/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:20:32 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/12/11 12:44:13 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int c)
{
	if ((c >= 9 && c <= 13) || (c == ' '))
	{
		return (8192);
	}
	else
		return (0);
}
/* 
#include <stdio.h>
#include <ctype.h>

int	main(void)
{

	printf("yay %i\n", isspace(' '));
	printf("yay %i", ft_isspace(' '));
	return (0);
} */