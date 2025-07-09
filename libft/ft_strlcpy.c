/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:52:10 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:09:57 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlen_strlcpy(char *str)
{
	int	i;

	i = 0;
	while (*(str + i) != '\0')
	{
		i++;
	}
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len_src;
	size_t	i;

	len_src = ft_strlen_strlcpy((char *)src);
	if (size == 0)
		return (len_src);
	i = 0;
	while (i < size - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (size > 0)
		dst[i] = '\0';
	return (len_src);
}
/* 
#include <bsd/string.h>
#include <stdio.h>

int	main(void)
{
	char dst[50] = "Hello, ";
	const char src[] = "lorem ipsum";
	
	char dst2[50] = "Hello, ";
	const char src2[] = "lorem ipsum";
	
	printf("Resulting length: %lu\n", ft_strlcpy(dst, src, 15));
	printf("my script: %s\n", dst);

	printf("2Resulting length: %lu\n", strlcpy(dst, src2, 15));
	printf("2original script: %s\n", dst);
	return 0;
} */