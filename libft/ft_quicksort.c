/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quicksort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:37:00 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/12/16 20:21:53 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static int	partition(int *arr, int low, int high)
{
	int	pivot;
	int	i;
	int	j;

	pivot = arr[high];
	i = low;
	j = low;
	while (j < high)
	{
		if (arr[j] <= pivot)
		{
			i++;
			swap(&arr[i - 1], &arr[j]);
		}
		j++;
	}
	swap(&arr[i], &arr[high]);
	return (i);
}

static void	quicksort_helper(int *arr, int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		quicksort_helper(arr, low, pi - 1);
		quicksort_helper(arr, pi + 1, high);
	}
}

static int	ft_alreadysorted(int *arr, int len)
{
	int	i;

	i = 1;
	while (i < len)
	{
		if (arr[i] > arr[i - 1])
			i++;
		else
			return (0);
	}
	return (1);
}

int	*ft_quicksort(int *arr, int len)
{
	if (arr == NULL || len <= 0)
		return (NULL);
	if (ft_alreadysorted(arr, len))
		return (arr);
	quicksort_helper(arr, 0, len - 1);
	return (arr);
}
/* 
#include <stdio.h>
int	main()
{
	int arr[] = {10, 7, 8, 9, 1, 5, 15};
	// int arr[] = {1, 2, 3, 4, 5, 6, 7};
	int n = sizeof(arr) / sizeof(arr[0]);
	quicksort(arr, n);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
	return 0;
} */