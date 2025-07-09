
#include "minirt.h"

int	main(void)
{
	t_data	data;
	t_mlx	mlx;

	data.mlx = &mlx;
	mlx.mlx_ptr = NULL;
	mlx.win_ptr = NULL;
	mlx.img_ptr = NULL;
	data.bg = 1;
	if (ft_window(&data) == NULL)
		return (1);
	return (0);
}