/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:38:09 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:38:38 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (lst && new)
	{
		last = ft_lstlast(*lst);
		if (!last)
			*lst = new;
		else
			last->next = new;
	}
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

int main()
{
	t_list *node1 = ft_lstnew(ft_strdup("4"));
	node1->next = ft_lstnew(ft_strdup("2"));
	node1->next->next = ft_lstnew(ft_strdup("21"));

	print_lst(node1);

	t_list *node2 = ft_lstnew(ft_strdup("node2"));
	ft_lstadd_back(&node1, node2);

	print_lst(node1);

	ft_lstclear(&node1, &del);
	return 0;
}*/