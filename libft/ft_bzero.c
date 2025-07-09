/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:30:42 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/17 11:48:56 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i != n)
	{
		ptr[i] = '\0';
		i++;
	}
}
/* 
#include <stdio.h>
#include <string.h> 

void printArray(char *arr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		printf("%c", arr[i]);
	}
}

int main() 
{ 
	char str[50] = "Hello this is a message from space"; 
	char str2[50] = "Hello this is a message from space"; 
	printf("\nBefore bzero():    %s\n", str); 
	printf("Before ft_bzero(): %s\n", str2); 

	bzero(str + 14, 7*sizeof(char)); 
	printf("After bzero():     ");
	printArray(str,34);	
	
	ft_bzero(str2 + 14, 7*sizeof(char)); 
	printf("\nAfter ft_bzero():  ");
	printArray(str2,34); 
	return 0; 
} 
*/
