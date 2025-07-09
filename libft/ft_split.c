/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:05:55 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 17:51:42 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_words_counter(const char *str, char c)
{
	size_t	counter;
	int		in_word;

	counter = 0;
	in_word = 0;
	while (*str)
	{
		if (!in_word && *str != c)
		{
			in_word = 1;
			counter++;
		}
		else if (in_word && *str == c)
			in_word = 0;
		str++;
	}
	return (counter);
}

static char	*ft_altstr_dup(const char *str, int start, int finish)
{
	char	*word;
	int		i;

	word = malloc((finish - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

static void	ft_free_split(char **split, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	**ft_splitcalc_split(const char *s, char c,
									char **split, size_t i)
{
	short int	index;
	size_t		j;

	index = -1;
	j = 0;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			split[j++] = ft_altstr_dup(s, index, i);
			if (!split[j - 1])
			{
				ft_free_split(split, j - 1);
				return (NULL);
			}
			index = -1;
		}
		i++;
	}
	split[j] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	word_count;
	char	**split;

	i = 0;
	if (!s)
		return (NULL);
	word_count = ft_words_counter(s, c);
	split = ft_calloc((word_count + 1), sizeof(char *));
	if (!split)
		return (NULL);
	return (ft_splitcalc_split(s, c, split, i));
}
/* 
#include <stdio.h>

int main()
{
	char *strings = "JANUARY,FEBRUARY,MARCH,APRIL,MAY,JUNE,JULY,
	AUGUST,SEPTEMBER,OCTOBER,NOVEMBER,DECEMBER,GLORBTEMBER";
	char** arrayX2;

	// printf("strings=[%s]\n\n", strings);

	arrayX2 = ft_split(strings, ',');
	
	
	if (arrayX2)
	{
		for (int i = 0; arrayX2[i]; i++)
		{
			printf("month=[%s]\n", arrayX2[i]);
			free(*(arrayX2 + i));
		}
		free(arrayX2);
	}
	else
		printf("arrayx2: %s\n\n", (char *)arrayX2);

	return 0;
} */

// static char	*ft_mallocwordlength_split(char *ptr, const char *s,
// 											char c, int *offset)
// {
// 	unsigned int	i;
// 	unsigned int	j;

// 	j = *offset;
// 	i = 0;
// 	while (s[j + i] != c && s[j + i] != '\0')
// 		i++;
// 	ptr = (char *)malloc((i + 1) * sizeof(char));
// 	if (!ptr)
// 		return (0);
// 	return (ptr);
// }

// static int	ft_countfind_split(const char *s, char c)
// {
// 	int	i;
// 	int	hel;

// 	i = 0;
// 	hel = 0;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] == c)
// 		{
// 			hel++;
// 		}
// 		i++;
// 	}
// 	return (hel + 1);
// }

// static int	ft_strncpy_split(char *dest, 
//								const char *src, char c, int *offset)
// {
// 	int	i;

// 	i = 0;
// 	while (src[*offset + i] != c && src[*offset + i] != '\0')
// 	{
// 		dest[i] = src[*offset + i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	*offset += i + 1;
// 	return (i);
// }

// static int	ft_free_split(char **ptrptr, char *ptr, int i)
// {
// 	i--;
// 	if (!ptr)
// 	{
// 		while (i >= 0)
// 		{
// 			free(ptrptr[i]);
// 			ptrptr[i] = NULL;
// 			i--;
// 		}
// 		free(ptrptr);
// 		return (1);
// 	}
// 	else
// 		return (0);
// }

// char	**ft_split(char const *s, char c)
// {
// 	int		i;
// 	int		j;
// 	char	*ptr;
// 	char	**ptrptr;

// 	i = 0;
// 	j = 0;
// 	if (s == 0 || c == 0)
// 		return (0);
// 	ptrptr = (char **)malloc((ft_countfind_split(s, c) + 1) * sizeof(char *));
// 	if (!ptrptr)
// 		return (0);
// 	while (i < ft_countfind_split(s, c))
// 	{
// 		ptr = ft_mallocwordlength_split(ptr, s, c, &j);
// 		if (ft_free_split(ptrptr, ptr, i))
// 			return (0);
// 		ft_strncpy_split(ptr, s, c, &j);
// 		ptrptr[i] = ptr;
// 		i++;
// 	}
// 	ptrptr[i] = NULL;
// 	return (ptrptr);
// }