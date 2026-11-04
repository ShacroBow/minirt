#include "../include/mesh.h"

/*
** Binned surface area heuristic: triangle centroids are sorted into
** BVH_BINS slots along an axis, and every boundary between two slots is a
** split candidate. Its cost is area(left) * count(left) + area(right) *
** count(right): the expected work for a ray that reaches this node.
*/

int	bin_of(const t_bvh_split *s, t_vec3 c)
{
	int	k;

	k = (int)((vec_axis(c, s->axis) - s->cmin) * s->scale);
	if (k < 0)
		return (0);
	if (k >= BVH_BINS)
		return (BVH_BINS - 1);
	return (k);
}

static void	fill_bins(t_bvh_build *b, const t_bvh_node *n, const t_bvh_split *s,
				t_bin *bins)
{
	int	i;
	int	k;
	int	tri;

	i = 0;
	while (i < BVH_BINS)
	{
		bins[i].box = aabb_empty();
		bins[i].count = 0;
		i++;
	}
	i = n->first;
	while (i < n->first + n->count)
	{
		tri = b->idx[i];
		k = bin_of(s, b->tc[tri]);
		bins[k].count++;
		aabb_merge(&bins[k].box, &b->tb[tri]);
		i++;
	}
}

/**
 * left.area[i] / left.count[i]: everything in bins 0..i.
 */
static void	sweep_left(const t_bin *bins, t_sweep *left)
{
	t_aabb	acc;
	int		count;
	int		i;

	acc = aabb_empty();
	count = 0;
	i = 0;
	while (i < BVH_BINS - 1)
	{
		aabb_merge(&acc, &bins[i].box);
		count += bins[i].count;
		left->area[i] = aabb_area(&acc);
		left->count[i] = count;
		i++;
	}
}

static void	sweep_right(const t_bin *bins, const t_sweep *left, t_bvh_split *s,
				t_bvh_split *best)
{
	t_aabb	acc;
	int		count;
	int		i;
	double	cost;

	acc = aabb_empty();
	count = 0;
	i = BVH_BINS - 1;
	while (i > 0)
	{
		aabb_merge(&acc, &bins[i].box);
		count += bins[i].count;
		cost = left->area[i - 1] * left->count[i - 1] + aabb_area(&acc) * count;
		if (left->count[i - 1] > 0 && count > 0 && cost < best->cost)
		{
			*best = *s;
			best->plane = i;
			best->cost = cost;
		}
		i--;
	}
}

/**
 * Cheapest split of node n over all three axes. False if the centroids are
 * all in one point (nothing to split).
 */
bool	find_split(t_bvh_build *b, const t_bvh_node *n, t_bvh_split *best)
{
	t_aabb		cb;
	t_bvh_split	s;
	t_bin		bins[BVH_BINS];
	t_sweep		left;
	double		extent;

	cb = centroid_bounds(b, n);
	best->cost = INFINITY;
	s.axis = 0;
	while (s.axis < 3)
	{
		s.cmin = vec_axis(cb.min, s.axis);
		extent = vec_axis(cb.max, s.axis) - s.cmin;
		if (extent > 1e-12)
		{
			s.scale = (double)BVH_BINS / extent;
			fill_bins(b, n, &s, bins);
			sweep_left(bins, &left);
			sweep_right(bins, &left, &s, best);
		}
		s.axis++;
	}
	return (best->cost < INFINITY);
}
