#include "../include/minirt.h"

/**
 * Called when the window becomes visible again (uncovered, restored,
 * focused). Only re-sends the last rendered image, nothing is re-traced.
 */
int	expose_hook(void *param)
{
	t_program	*prog;

	prog = (t_program *)param;
	if (prog->mlx.mlx_ptr && prog->mlx.win_ptr && prog->mlx.img_ptr)
		mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
			prog->mlx.img_ptr, 0, 0);
	return (0);
}
