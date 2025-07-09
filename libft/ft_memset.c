/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:50:28 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/11 19:08:24 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i != n)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
/* 
#include <stdio.h> 
#include <string.h> 
int main() 
{ 
	char str[50] = "Hello this is a message from space"; 
	char str2[50] = "Hello this is a message from space"; 
	char *ptr;
	printf("\nBefore memset():    %s\n", str); 
	printf("Before ft_memset(): %s\n", str2); 


	ptr = memset(str + 14, '0', 9*sizeof(char)); 
	printf("After memset():     %s\n", ptr);

	ptr = ft_memset(str2 + 14, '0', 9*sizeof(char)); 
	printf("After ft_memset():  %s\n", ptr); 
	return 0; 
} */
