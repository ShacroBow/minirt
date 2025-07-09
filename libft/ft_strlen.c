/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:12:04 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/19 12:11:03 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (*str != '\0')
	{
		i++;
		str++;
	}
	return (i);
}
/* 
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*s;

	s = "Hello_! \0 hi ";
	printf("%ld\n", strlen(s));
	printf("%ld", ft_strlen(s));

	return (0);
} */
