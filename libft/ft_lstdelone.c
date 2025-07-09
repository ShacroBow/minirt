/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:40:07 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/24 15:40:14 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
/* 
#include <stdio.h>
static void del(void *content)
{
	free(content);
}

int main()
{
	char *ptr = "Hello, World!";
	t_list *node = ft_lstnew(malloc( ft_strlen(ptr) * sizeof(char)));
	if (node) {
		ft_memcpy(node->content, ptr, ft_strlen(ptr));
		printf("Node content before deletion: %s\n", (char *)node->content);
		ft_lstdelone(node, &del);
	}
	return 0;
} */