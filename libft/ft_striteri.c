/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:22:56 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 18:17:24 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (s != 0 && f != 0)
	{
		while (s[i] != '\0')
		{
			f(i, (s + i));
			i++;
		}
		s[i] = '\0';
	}
}
/* 
#include <stdio.h>
#include <stdlib.h>

static void	ft_asciiadd(unsigned int b, char *c)
{
	if (*c >= 97 && *c <= 122 && (*c + b) < 127)
	{
		*c = *c + b;
	}
	else if (*c >= 97 && *c <= 122 && (*c + b) >= 127)
	{
		*c = *c - b;
	}
}

int main()
{
	char string[] = "This is a MESSAGE from the STARs.";
	void (*funco)(unsigned int, char *);
	printf("\noriginal string : %s\n", string);
	
	funco = ft_asciiadd;
	ft_striteri(string, funco);
	printf("my string       : %s\n", string);
	
	return (0);
} */