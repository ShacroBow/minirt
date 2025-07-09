/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:53:34 by kmashkoo          #+#    #+#             */
/*   Updated: 2025/04/04 18:54:03 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	*ptr2;

	i = 0;
	ptr = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	while (ptr[i] != '\0' && ptr2[i] != '\0')
	{
		if (ptr[i] != ptr2[i])
		{
			return (ptr[i] - ptr2[i]);
		}
		i++;
	}
	return (ptr[i] - ptr2[i]);
}

/* #include <stdio.h>
#include <string.h>
int	main(void)
{
	char str1[] = "abcdefgh";
	char str2[] = "abcdwxyz";

	int result = ft_strcmp(" ", "  ");
	printf("The strings is:%d.\n", result);

	result = strcmp(" ", "  ");
	printf("The original strings is:%d.\n", result);

	return 0;
} */