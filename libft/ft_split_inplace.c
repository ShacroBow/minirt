/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_inplace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 19:37:01 by kmashkoo          #+#    #+#             */
/*   Updated: 2026/01/01 19:54:57 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	loop_splitter(t_split *d)
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

static int	loop_word(t_split *d, size_t *count)
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

/* RETURN: the address of the ith string, from a string
	split by ft_split_inplace(char *str, char c).

   NOTES: Do not pass out of bound index.*/
char	*index_split(char *str, size_t index)
{
	size_t	i;
	char	*res;

	i = 0;
	res = str;
	while (res[i] == 0)
		res++;
	while (index)
	{
		res += ft_strlen(res) + 1;
		index--;
	}
	return (res);
}

size_t	ft_split_inplace(char *str, char c)
{
	t_split	d;
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
//	//	while (str[i] == 0)
//	//		i++;
//	//	if (i < count)
//	//		return (count)
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
// 	(void)!write(2, "]\n", 2);
// 	return (0);
// }
