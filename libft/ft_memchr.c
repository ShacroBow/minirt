/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:42:47 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:04:17 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
		{
			return (ptr + i);
		}
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>

int main() 
{
	char str[50] = "Hello, world! This is Helo a test.";
	char *result;
	
	result = ft_memchr(str, '4', 12);
	printf("1my script       %s\n", result);
	
	result = ft_memchr(str, 'T', 11);
	printf("2my script       %s\n", result);

	result = ft_memchr(str, '.', 40);
	printf("3my script       %s\n", result);
 
	result = ft_memchr(str, 'T', 26);
	printf("4my script       %s\n", result);

	result = ft_memchr(str, 'w', 30);
	printf("5my script       %s\n", result);
	
	result = ft_memchr(str, '!', 30);
	printf("6my script       %s\n\n", result);

	result = memchr(str, '4', 12);
	printf("1original script %s\n", result);
	
	result = memchr(str, 'T', 11);
	printf("2original script %s\n", result);
	
	result = memchr(str, '.', 40);
	printf("3original script %s\n", result);

	result = memchr(str, 'T', 26);
	printf("4original script %s\n", result);

	result = memchr(str, 'w', 30);
	printf("5original script %s\n", result);
	
	result = memchr(str, '!', 30);
	printf("6original script %s\n", result);
	return 0;
} */