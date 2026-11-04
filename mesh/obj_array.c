#include "../include/mesh.h"

/**
 * Appends one element (a->size bytes) to a growable array. Running out of
 * memory is fatal for the loader.
 */
void	arr_push(t_objfile *f, t_arr *a, const void *elem)
{
	void	*bigger;
	size_t	cap;

	if (a->len == a->cap)
	{
		cap = a->cap * 2;
		if (cap == 0)
			cap = 64;
		bigger = malloc(cap * a->size);
		if (!bigger)
			obj_fatal(f, "out of memory");
		if (a->data)
			ft_memcpy(bigger, a->data, a->len * a->size);
		free(a->data);
		a->data = bigger;
		a->cap = cap;
	}
	ft_memcpy((char *)a->data + a->len * a->size, elem, a->size);
	a->len++;
}

/**
 * Prints "Warning: <path>: <msg> '<what>'" to stderr. Warnings are for things
 * that can be skipped (unknown statements, missing .mtl or textures).
 */
void	obj_warn(const char *path, const char *msg, const char *what)
{
	ft_putstr_fd("Warning: ", 2);
	ft_putstr_fd((char *)path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd((char *)msg, 2);
	if (what)
	{
		ft_putstr_fd(" '", 2);
		ft_putstr_fd((char *)what, 2);
		ft_putstr_fd("'", 2);
	}
	ft_putstr_fd("\n", 2);
}
