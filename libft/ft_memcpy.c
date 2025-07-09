/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:10:40 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 18:56:35 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		((char *)dest)[i] = ((char *)src)[i];
		++i;
	}
	return (dest);
}
/*
#include <string.h> 
#include <stdio.h>
int main() 
{ 
	char str[50] = "the magic number is three"; 
	char str2[50] = "fourty two"; 
	char *ptr;
	
	printf("Before str memcpy():  %s\n", str); 
	printf("Before str2 memcpy(): %s\n\n", str2); 


	ptr = (char *)memcpy(str+20, str2, 10); 
	printf("After memcpy():       %s\n", ptr);
	printf("plan string:          %s\n\n", str);

	ptr = (char *)ft_memcpy(str+20, str2+7, 3); 
	printf("After ft_memcpy():    %s\n", ptr); 
	printf("plan string:          %s\n", str);
	return 0; 
} */
