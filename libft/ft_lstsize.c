/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:42:09 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:42:12 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	counter;

	counter = 0;
	while (lst)
	{
		lst = lst->next;
		counter++;
	}
	return (counter);
}
/* 
#include <stdio.h>
static void del(void *content)
{
	free(content);
}

int main() {
	t_list *list = NULL;

	list = ft_lstnew(ft_strdup("Node 1"));
	list->next = ft_lstnew(ft_strdup("Node 2"));
	list->next->next = ft_lstnew(ft_strdup("Node 3"));

	int size = ft_lstsize(list);
	printf("List size: %d\n", size);
	ft_lstclear(&list, &del);

	return 0;
} */
