/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:30:20 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 18:55:44 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	*ptr2;

	i = 0;
	ptr = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	while (ptr[i] == ptr2[i] && i + 1 < n)
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
/* #include <string.h>
#include <stdio.h>

int	main(void)
{
	char str1[] = "Hell\0'o'";
	char str2[] = "Hell\0'0'";

	int result = ft_memcmp(str1, str2, 8);
	
	printf("my strings is: %d.\n", result);
	
	int result2 = memcmp(str1, str2, 8);
	printf("original strings is: %d.\n", result2);
	return 0;
} */