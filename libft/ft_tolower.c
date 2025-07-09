/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 20:13:31 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 13:21:38 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
	{
		return (c + 32);
	}
	return (c);
}
/* 
#include <stdio.h>
#include <ctype.h>
int main()
{
	printf("My script	%c\n", ft_tolower('E'));
	printf("original	%c\n", tolower('E')); ;
} */