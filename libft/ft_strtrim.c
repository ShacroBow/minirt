/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:57:09 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 17:48:36 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trim;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && s1[end - 1] && ft_strchr(set, s1[end - 1]))
		end--;
	trim = malloc(sizeof(char) * (end - start + 1));
	if (trim)
		ft_strlcpy(trim, &s1[start], end - start + 1);
	return (trim);
}
/* 
#include <stdio.h>

int main()
{
	// char *string = "easypeasy.com";
	// char *set = ".com";
	// char *set2 = "easy";
	// char *string = "HelloHilHiHello";
	// char *set = "Hello";
	// char *set2 = "Hi";
	char *string = "xXDARKxNESSXx";
	char *set = "x";
	char *set2 = "X";
	char *ptr;
	char *ptr2;
	
	printf("string untrimmed: %s\n", string); 

	ptr = ft_strtrim(string, set);
	printf("string trimmed:   %s\n", ptr);
	
	ptr2 = ft_strtrim(ptr, set2);
	printf("string trimmed:   %s\n", ptr2);
	free(ptr);
	free(ptr2);
	return 0;
} */
// static char	*ft_strrev(const char *str)
// {
// 	size_t	len;
// 	size_t	i;
// 	char	*reversed;

// 	i = 0;
// 	len = 0;
// 	if (!str)
// 		return (NULL);
// 	while (str[len] != '\0')
// 		len++;
// 	reversed = (char *)malloc(len + 1);
// 	if (!reversed)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		reversed[i] = str[(len - 1) - i];
// 		i++;
// 	}
// 	reversed[len] = '\0';
// 	return (reversed);
// }

// static char	*ft_strcheck_strtrim(const char *big,
// 								const char *little, size_t len)
// {
// 	size_t	i;

// 	if (big[0] == '\0')
// 		return ((char *)big);
// 	i = 0;
// 	while (big[i] != '\0' && big[i] == little[i] && i < len)
// 	{
// 		if (little[i + 1] == '\0')
// 			return ((char *)big + i);
// 		++i;
// 	}
// 	return (0);
// }

// static char	*ft_supermalloc_strtrim(int k, int i, int j)
// {
// 	char	*ptr;

// 	if (k == 3)
// 		ptr = (char *)ft_calloc((i - (j * 2)) + 1, sizeof(char));
// 	if (k == 2 || k == 1)
// 		ptr = (char *)ft_calloc((i - j) + 1, sizeof(char));
// 	if (k == 0)
// 		ptr = (char *)ft_calloc((i + 1), sizeof(char));
// 	else if (k < 0 || k > 3)
// 		return (0);
// 	if (!ptr)
// 		return (0);
// 	return (ptr);
// }

// static char	*ft_strcpy_trim(const char *s1, int k, int i, int j)
// {
// 	int		hel;
// 	char	*ptr;

// 	hel = 0;
// 	ptr = ft_supermalloc_strtrim(k, i, j);
// 	if (!ptr)
// 		return (NULL);
// 	while (hel < i && (k >= 0 && k <= 3))
// 	{
// 		if (k == 3 && hel < (i - (j * 2)))
// 			ptr[hel] = s1[hel + j];
// 		else if (k == 2 && hel < i - j)
// 			ptr[hel] = s1[hel];
// 		else if ((k == 1) && hel < i - j)
// 			ptr[hel] = s1[hel + j];
// 		else if (k == 0)
// 			ptr[hel] = s1[hel];
// 		hel++;
// 	}
// 	return (ptr);
// }

// char	*ft_strtrim(char const *s1, char const *set)
// {
// 	int		j;
// 	int		k;
// 	char	*ptr;

// 	if (!s1 || !set)
// 		return (0);
// 	j = ft_strlen(set);
// 	k = 0;
// 	if (ft_strcheck_strtrim(s1, set, j))
// 		k += 1;
// 	ptr = ft_strrev(s1);
// 	if (ft_strcheck_strtrim(s1 + (ft_strlen(s1) - j), set, j)
// 		|| ft_strcheck_strtrim(ptr, set, j))
// 		k += 2;
// 	if (ptr)
// 		free(ptr);
// 	ptr = ft_strcpy_trim(s1, k, ft_strlen(s1), j);
// 	return (ptr);
// }