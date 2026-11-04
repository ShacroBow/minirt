#include "../include/minirt.h"

/**
 * Scrambles all bits of h (splitmix64 finalizer), so nearby inputs give
 * unrelated random sequences.
 */
size_t	mix_seed(size_t h)
{
	h ^= h >> 33;
	h *= (size_t)0xff51afd7ed558ccdUL;
	h ^= h >> 33;
	h *= (size_t)0xc4ceb9fe1a85ec53UL;
	h ^= h >> 33;
	return (h);
}

size_t	pixel_seed(int x, int y, int sample)
{
	return (mix_seed(((size_t)x * 73856093UL)
			^ ((size_t)y * 19349663UL)
			^ ((size_t)sample * 83492791UL)));
}

/**
 * Hash of a surface point that stays defined for any coordinate.
 */
size_t	point_hash(const t_point *p)
{
	return (((size_t)(long)fmod(p->x * 4096.0, 1e15) * 73856093UL)
		^ ((size_t)(long)fmod(p->y * 4096.0, 1e15) * 19349663UL)
		^ ((size_t)(long)fmod(p->z * 4096.0, 1e15) * 83492791UL));
}

/**
 * Random number in [0, 1) for a surface point, different every sample.
 */
double	point_random(const t_point *p, int sample)
{
	unsigned long	seed;

	seed = mix_seed(point_hash(p) + (size_t)sample);
	return (fast_rand(&seed));
}
