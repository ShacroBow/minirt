#include "../include/minirt.h"

/**
 * minilibx-linux allocates the display connection in mlx_init(), so it has
 * to be closed with mlx_destroy_display() and the struct freed afterwards.
 */
void	platform_destroy_mlx(void *mlx_ptr)
{
	mlx_destroy_display(mlx_ptr);
	free(mlx_ptr);
}
