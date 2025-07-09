/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:23 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 15:40:13 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static char	*ft_handlethisoneannoyingthingonly_itoa(int n)
{
	int		i;
	char	*message;
	char	*ptr;

	if (n == -2147483648)
	{
		message = "-2147483648";
		ptr = (char *)malloc(12);
		if (!ptr)
			return (NULL);
		i = 0;
		while (message[i] != '\0')
		{
			ptr[i] = message[i];
			i++;
		}
		ptr[i] = '\0';
		return (ptr);
	}
	return (0);
}

static int	ft_intlen_itoa(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
	{
		i = 1;
	}
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static void	ft_revstr_itoa(char *str, int size)
{
	int		i_start;
	int		i_end;
	char	carry;

	i_start = 0;
	i_end = size - 1;
	while (i_start < i_end)
	{
		carry = str[i_start];
		str[i_start] = str[i_end];
		str[i_end] = carry;
		i_start++;
		i_end--;
	}
}

static char	*ft_itoacalc_itoa(char *ptr, int n, int digit, int negativecheck)
{
	if (n == 0)
	{
		ptr[digit] = '0';
		digit++;
	}
	else
	{
		while (n > 0)
		{
			ptr[digit] = (n % 10) + 48;
			n /= 10;
			digit++;
		}
	}
	if (negativecheck == 1)
	{
		ptr[digit] = '-';
		digit++;
	}
	ptr[digit] = '\0';
	ft_revstr_itoa(ptr, digit);
	return (ptr);
}

char	*ft_itoa(int n)
{
	int		numsize;
	int		digit;
	int		negativecheck;
	char	*ptr;

	numsize = ft_intlen_itoa(n);
	ptr = (char *)malloc(numsize + 1);
	if (!ptr)
		return (0);
	negativecheck = 0;
	digit = 0;
	if (n == -2147483648)
	{
		free(ptr);
		return (ft_handlethisoneannoyingthingonly_itoa(n));
	}
	else if (n < 0)
	{
		negativecheck = 1;
		n = -n;
	}
	ptr = ft_itoacalc_itoa(ptr, n, digit, negativecheck);
	return (ptr);
}
/* #include <stdio.h>

int main(void)
{
	char *ptr;

	ptr = ft_itoa(10);
	printf("ft_itoa: %s\n", ptr);
	free(ptr);

	ptr = ft_itoa(-5);
	printf("ft_itoa: %s\n", ptr);
	free(ptr);

	ptr = ft_itoa(50341);
	printf("ft_itoa: %s\n", ptr);
	free(ptr);

	ptr = ft_itoa(0);
	printf("ft_itoa: %s\n", ptr);
	free(ptr);

	ptr = ft_itoa(2147483647);
	printf("ft_itoa: %s\n", ptr);
	free(ptr);

	ptr = ft_itoa(-2147483648);
	printf("ft_itoa: %s\n", ptr);
	free(ptr);

	return 0;
} */