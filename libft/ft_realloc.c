/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 16:58:02 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/12/24 16:58:15 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size, size_t type)
{
	void	*new_ptr;

	if (new_size <= 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = ft_calloc(new_size, type);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (new_ptr);
	if (new_size > old_size)
		ft_memcpy(new_ptr, ptr, old_size);
	else
		ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}
//realloc-ing non malloc-ed memory will seg fault.
