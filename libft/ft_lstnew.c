/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:41:51 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:41:54 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (node == NULL)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
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

	print_lst(node1);
	ft_lstclear(&node1, &del);
	print_lst(node1);
	return 0;
} */