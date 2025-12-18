/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_inplace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:31 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/12/18 16:05:31 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	loop_splitter(split_t *d)
{
	while (d->str[d->i] == d->c)
	{
		if (d->in_word)
			d->in_word = 0;
		d->str[d->i] = 0;
		d->i++;
		if (d->i == d->strlen)
			return (1);
	}
	return (0);
}

int	loop_word(split_t *d, size_t *count)
{
	while (d->str[d->i] != d->c)
	{
		if (!d->in_word)
		{
			*count += 1;
			d->in_word = 1;
		}
		d->i++;
		if (d->i == d->strlen)
			return (1);
	}
	return (0);
}

size_t	ft_split_inplace(char *str, char c)
{
	split_t	d;
	size_t	count;

	count = 0;
	d.str = str;
	d.c = c;
	d.i = 0;
	d.in_word = 0;
	d.strlen = ft_strlen(str);
	if (d.strlen == 0)
		return (0);
	while (d.i < d.strlen)
	{
		if (loop_splitter(&d))
			return (count);
		if (loop_word(&d, &count))
			return (count);
	}
	return (count);
}

// #include <unistd.h>
// #include <stdio.h>
// int	main(void)
// {
// 	char *str = ft_strdup("this will be the result");
// 	size_t	strlen = ft_strlen(str);
// 	printf("SPLITTER: (%c)\nOG STR:\n[%s]\n", ' ', str);

// 	size_t word_len = 0;
// 	size_t count = ft_split_inplace(str, ' ');

// 	// // EXAMPLE OF LOOPING RESULTING STRINGS:
// 	// for (size_t i = 0; i < count; i++)
// 	// {
// 	// 	word_len = ft_strlen(str);
// 	// 	write(1, str, word_len);
// 	// 	write(1, "\n", 1);
// 	// 	str += word_len + 1;		
// 	// }

// 	printf("\nRESULTING STR:\n");
// 	write(1, "[", 1);
// 	for (size_t i = 0; i < strlen; i++)
// 	{
// 		if (str[i] == 0)
// 			write(1, "_", 1);
// 		else
// 			write(1, &str[i], 1);
// 	}
// 	write(2, "]\n", 2);
// 	return (0);
// }
