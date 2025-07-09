/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:31:04 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:17:03 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*ptr;

	i = 0;
	ptr = (char *)big;
	if (little[0] == '\0')
		return (ptr);
	if (len == 0 || ft_strlen(big) < ft_strlen(little))
		return (NULL);
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while (big[i + j] != '\0' && big[i + j] == little[j] && (i + j) < len)
		{
			if (little[j + 1] == '\0')
				return (ptr + i);
			++j;
		}
		++i;
	}
	return (NULL);
}
/* 
#include <stdio.h>
#include <bsd/string.h>

int main() 
{
	char str[50] = "Hello, world! This is Helo a test.";
	char *result;
	
	result = ft_strnstr(str, "world", 12);
	printf("1my script       %s\n", result);
	
	result = ft_strnstr(str, "world", 11);
	printf("2my script       %s\n", result);

	result = ft_strnstr(str, "his is Helo a tesg", 40);
	printf("3my script       %s\n", result);
 
	result = ft_strnstr(str, "Helo", 26);
	printf("4my script       %s\n", result);

	result = ft_strnstr(str, "HELLO", 30);
	printf("5my script       %s\n", result);
	
	result = ft_strnstr("", "coucou", 15);
	printf("6my script       %s\n\n", result);

	result = strnstr(str, "world", 12);
	printf("1original script %s\n", result);
	
	result = strnstr(str, "world", 11);
	printf("2original script %s\n", result);
	
	result = strnstr(str, "his is Helo a tesg", 40);
	printf("3original script %s\n", result);

	result = strnstr(str, "Helo", 26);
	printf("4original script %s\n", result);

	result = strnstr(str, "HELLO", 30);
	printf("5original script %s\n", result);
	
	result = strnstr("", "coucou", 15);
	printf("6original script %s\n", result);
	return 0;
} */