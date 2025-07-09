/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:01:32 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:07:21 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;

	i = 0;
	if (!dest && !src)
		return (dest);
	if (dest == src)
		return (dest);
	if (dest < src)
	{
		while (i < n)
		{
			((char *)dest)[i] = ((const char *)src)[i];
			i++;
		}
		return (dest);
	}
	i = n;
	while (i > 0)
	{
		i--;
		((char *)dest)[i] = ((const char *)src)[i];
	}
	return (dest);
}
/* 
#include <stdio.h>
#include <string.h> 

int main() 
{ 
	char str[50] = "the magic number is fourty two"; 
	char *ptr;
	printf("\nBefore str memmove():  %s\n", str); 


	ptr = (char *)memmove(str+10, str+4, 40); 
	printf("After memmove():       %s\n", ptr);
	printf("plan string:           %s\n\n", str);

	ptr = (char *)ft_memmove(str+10, str+4, 40); 
	printf("After ft_memmove():    %s\n", ptr); 
	printf("plan string:           %s\n", str);
	return 0; 
} */
