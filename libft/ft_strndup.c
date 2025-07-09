/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:56:55 by kmashkoo          #+#    #+#             */
/*   Updated: 2025/04/19 20:56:57 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*dup;
	size_t	i;

	len = 0;
	while (s[len] && len < n)
		len++;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		++i;
	}
	dup[len] = '\0';
	return (dup);
}

/* 
#include <stdio.h>

int	main(void)
{
	const char	*src = "Hello, world!";
	size_t		n   = 7;
	char		*dup = ft_strndup(src, n);
	if (!dup)
		return (1);
	printf("Original : \"%s\"\n", src);
	printf("Duplicate : \"%s\"\n", dup);
	free(dup);
	return (0);
}
 */
