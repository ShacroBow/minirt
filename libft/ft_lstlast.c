/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:35 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:40:52 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}
/* 
#include <stdio.h>

void print_lst(t_list *lst)
{
	while (lst)
	{
		printf("%s -> ", (char *)lst->content);
		lst = lst->next;
	}
	printf("NULL\n");
}

static void del(void *content)
{
	free(content);
}

int main() {
	t_list *node1 = ft_lstnew(ft_strdup("4"));
	node1->next = ft_lstnew(ft_strdup("2"));
	node1->next->next = ft_lstnew(ft_strdup("21"));
	t_list *ptr;

	print_lst(node1);
	ptr = ft_lstlast(node1);
	print_lst(ptr);

	ft_lstclear(&node1, &del);
	return 0;
} */