/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:22:56 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 18:14:04 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static size_t	ft_strlen_strmapi(const char *str)
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

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*ptr;

	if (s == 0 || f == 0)
		return (0);
	i = 0;
	ptr = (char *)malloc(ft_strlen_strmapi(s) + 1);
	if (ptr == 0)
		return (0);
	while (i < ft_strlen_strmapi(s))
	{
		ptr[i] = f(i, s[i]);
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
/* 
#include <stdio.h>

static char	ft_asciiadd(unsigned int b, char c)
{
	char	charret;

	if (c >= 97 && c <= 122 && (c + b) < 127)
	{
		charret = c + b;
		return (charret);
	}
	else if (c >= 97 && c <= 122 && (c + b) >= 127)
	{
		charret = c - b;
		return (charret);
	}
	return (c);
}

int main()
{
	char *string = "This is a MESSAGE from the STARs.";
	char *ptr;
	char (*funco)(unsigned int, char);
	printf("\noriginal string : %s\n", string);
	
	funco = ft_asciiadd;
	ptr = ft_strmapi(string, funco);
	printf("my string       : %s\n", ptr);
	free(ptr);
	
	return (0);
} */