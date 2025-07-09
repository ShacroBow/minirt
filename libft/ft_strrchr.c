/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:37:57 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/19 20:14:34 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*ptr;

	ptr = (char *)s;
	i = 0;
	while (ptr[i] != '\0')
	{
		i++;
	}
	if (ptr[i] == (unsigned char)c)
		return (ptr + i);
	while (i != 0)
	{
		if (ptr[i - 1] == (unsigned char)c)
		{
			return (ptr + i - 1);
		}
		i--;
	}
	return (0);
}
/* 
#include <string.h>
#include <stdio.h>

int main() 
{
	char str[50] = "Hello, world! This is Helo a test.";
	char *result;

	result = ft_strrchr(str, '4');
	printf("1my script       %s\n", result);

	result = ft_strrchr(str, 'H');
	printf("2my script       %s\n", result);

	result = ft_strrchr(str, '.');
	printf("3my script       %s\n", result);
 
	result = ft_strrchr(str, 'T');
	printf("4my script       %s\n", result);

	result = ft_strrchr(str, 'w');
	printf("5my script       %s\n", result);

	result = ft_strrchr("teste", '\0');
	printf("6my script       %s\n\n", result);

	result = strrchr(str, '4');
	printf("1original script %s\n", result);
	
	result = strrchr(str, 'H');
	printf("2original script %s\n", result);
	
	result = strrchr(str, '.');
	printf("3original script %s\n", result);

	result = strrchr(str, 'T');
	printf("4original script %s\n", result);

	result = strrchr(str, 'w');
	printf("5original script %s\n", result);
	
	result = strrchr("teste", '\0');
	printf("6original script %s\n", result);
	return 0;
} */