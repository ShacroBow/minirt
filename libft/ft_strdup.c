/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:15:13 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 18:44:54 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static size_t	ft_strlen_strdup(const char *str)
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

static char	*ft_strcpy_strdup(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*dst;

	dst = (char *)malloc(ft_strlen_strdup (s) + 1);
	if (!dst)
		return (0);
	return (ft_strcpy_strdup(dst, s));
}
/* #include <stdio.h>
#include <string.h>

int main() {
	const char *original = "Hello, World!";
	char *duplicate;

	duplicate = strdup(original);
	printf("Og Original: %s\n", original);
	printf("Og Duplicate: %s\n\n", duplicate);
	free(duplicate);

	duplicate = ft_strdup(original);
	printf("my Original: %s\n", original);
	printf("my Duplicate: %s\n", duplicate);
	free(duplicate);

	return 0;
} */