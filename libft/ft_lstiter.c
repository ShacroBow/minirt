/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:24 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:40:27 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst && f)
	{
		while (lst)
		{
			f(lst->content);
			lst = lst->next;
		}
	}
}
/* 
#include <stdio.h>
void ft_toupper_lstiter(char *c)
{
	int i;

	i = 0;
	while (c[i] != '\0')
	{
		if (c[i] >= 97 && c[i] <= 122)
		{
			c[i] = (c[i] - 32);
		}
		i++;
	}
}

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
	t_list *node1 = ft_lstnew(ft_strdup("4Heyoo"));
	node1->next = ft_lstnew(ft_strdup("2I am lowerCASE"));
	node1->next->next = ft_lstnew(ft_strdup("21loolkatme"));

	print_lst(node1);
	ft_lstiter(node1, (void *)ft_toupper_lstiter);
	print_lst(node1);
	ft_lstclear(&node1, &del);
	return 0;
} */