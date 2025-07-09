/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:39:03 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:39:55 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*copy;

	if (lst && *lst && del)
	{
		while (*lst)
		{
			copy = (*lst)->next;
			ft_lstdelone(*lst, del);
			*lst = copy;
		}
		*lst = NULL;
	}
}
/* 
#include <stdio.h>

static void del(void *content)
{
	free(content);
}

void print_list(t_list *lst)
{
	while (lst) {
		printf("%s -> ", (char *)lst->content);
		lst = lst->next;
	}
	printf("NULL\n");
}
int main() {
	t_list *list = NULL;

	list = ft_lstnew(ft_strdup("Node 1"));
	list->next = ft_lstnew(ft_strdup("Node 2"));
	list->next->next = ft_lstnew(ft_strdup("Node 3"));

	print_list(list);
	ft_lstclear(&list, &del);
	print_list(list);
	return 0;
} */