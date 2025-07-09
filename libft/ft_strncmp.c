/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:49:19 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 18:59:22 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	*ptr2;

	i = 0;
	ptr = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	while (i < n && ptr[i] != '\0' && ptr2[i] != '\0')
	{
		if (ptr[i] != ptr2[i])
		{
			return (ptr[i] - ptr2[i]);
		}
		i++;
	}
	if (i < n)
	{
		return (ptr[i] - ptr2[i]);
	}
	return (0);
}
/* 
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char str1[] = "abcdefgh";
	char str2[] = "abcdwxyz";

	int result = ft_strncmp("test\200", "test\0", 6);
	printf("The strings is:%d.\n", result);


	result = strncmp("test\200", "test\0", 6);
	printf("The original strings is:%d.\n", result);

	return 0;
} */