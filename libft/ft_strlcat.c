/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:52:10 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 18:19:28 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlen_strlcat(char *str)
{
	int	i;

	i = 0;
	while (*(str + i) != '\0')
	{
		i++;
	}
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	rem;
	size_t	i;

	len_dst = ft_strlen_strlcat(dst);
	len_src = ft_strlen_strlcat((char *)src);
	rem = 0;
	i = 0;
	if (size > len_dst)
		rem = len_src + len_dst;
	else
		rem = len_src + size;
	while (src[i] && (len_dst + i + 1) < size)
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (rem);
}
/* 
#include <bsd/string.h>
#include <stdio.h>

int	main(void)
{
	char dst[50] = "Hello, ";
	const char src[] = "world!";
	
	char dst2[50] = "Hello, ";
	const char src2[] = "world!";
	
	printf("Resulting length: %lu\n", ft_strlcat(dst, src, 11));
	printf("my script: %s\n", dst);

	printf("2Resulting length: %lu\n", strlcat(dst2, src2, 11));
	printf("2original script: %s\n", dst);
	return 0;
} */