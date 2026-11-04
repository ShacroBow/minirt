#include "../include/mesh.h"

t_aabb	centroid_bounds(t_bvh_build *b, const t_bvh_node *n)
{
	t_aabb	cb;
	int		i;

	cb = aabb_empty();
	i = n->first;
	while (i < n->first + n->count)
	{
		aabb_grow(&cb, b->tc[b->idx[i]]);
		i++;
	}
	return (cb);
}

/**
 * Reorders the node's triangle indices so the ones left of the split plane
 * come first. Returns how many went left.
 */
int	bvh_partition(t_bvh_build *b, const t_bvh_node *n, const t_bvh_split *s)
{
	int	i;
	int	j;
	int	tmp;

	i = n->first;
	j = n->first + n->count - 1;
	while (i <= j)
	{
		if (bin_of(s, b->tc[b->idx[i]]) < s->plane)
			i++;
		else
		{
			tmp = b->idx[i];
			b->idx[i] = b->idx[j];
			b->idx[j] = tmp;
			j--;
		}
	}
	return (i - n->first);
}

/**
 * Sets node to hold triangles [first, first + count) and computes its box.
 */
void	node_init(t_bvh_build *b, int node, int first, int count)
{
	t_bvh_node	*n;
	int			i;

	n = &b->m->nodes[node];
	n->first = first;
	n->count = count;
	n->box = aabb_empty();
	i = first;
	while (i < first + count)
	{
		aabb_merge(&n->box, &b->tb[b->idx[i]]);
		i++;
	}
}

/**
 * Frees the temporary build arrays and stops loading.
 */
void	bvh_fail(t_bvh_build *b, const char *msg)
{
	free(b->tb);
	free(b->tc);
	free(b->idx);
	obj_fatal(b->f, msg);
}
