/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 20:09:00 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/10 16:28:06 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
	{
		return (c - 32);
	}
	return (c);
}
/* 
#include <stdio.h>
#include <ctype.h>
int main()
{
	printf("My script	%c\n", ft_toupper('e'));
	printf("original	%c\n", toupper('e')); ;
} */