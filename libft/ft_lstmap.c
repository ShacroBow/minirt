/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:41:02 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:41:37 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_cont;
	void	*ptr;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		ptr = f(lst->content);
		new_cont = ft_lstnew(ptr);
		if (!new_cont)
		{
			ft_lstclear(&new_lst, del);
			del(ptr);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_cont);
		lst = lst->next;
	}
	return (new_lst);
}
/* #include <stdio.h>

void *ft_toupper_lstmap(void *ptr)
{
	char *c = (char *)ptr;
	char *new_str = ft_strdup(c);
	int i = 0;

	while (new_str[i] != '\0')
	{
		if (new_str[i] >= 'a' && new_str[i] <= 'z')
		{
			new_str[i] = new_str[i] - 32;
		}
		i++;
	}
	return (new_str);
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

int main()
{
	t_list *node1 = ft_lstnew(ft_strdup("4Heyoo"));
	node1->next = ft_lstnew(ft_strdup("2I am lowerCASE"));
	node1->next->next = ft_lstnew(ft_strdup("21loolkatme"));
	t_list *ptr;
	
	printf("Base:\n");
	print_lst(node1);
	ptr = ft_lstmap(node1, &ft_toupper_lstmap, del);
	printf("\nboth functional:\n");
	print_lst(node1);
	print_lst(ptr);
	ft_lstclear(&node1, &del);
	printf("\nonly ptr:\n");
	print_lst(node1);
	print_lst(ptr);
	ft_lstclear(&ptr, &del);
	printf("\nneither\n");
	print_lst(node1);
	print_lst(ptr);
	return 0;
} */