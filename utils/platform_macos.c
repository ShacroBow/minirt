#include "../include/minirt.h"

/**
 * The Metal MiniLibX has no mlx_destroy_display(). The pointer returned by
 * mlx_init() is a Swift object that lives until the process exits, so it
 * must not be passed to free() either.
 */
void	platform_destroy_mlx(void *mlx_ptr)
{
	(void)mlx_ptr;
}
