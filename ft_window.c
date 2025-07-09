#include "minirt.h"

int	ft_keyhandler(int keycode, t_data *data)
{
	if (keycode == KEY_ESC)
		ft_destroyhandler(data->mlx);
	return (0);
}

int	ft_destroyhandler(t_mlx *mlx)
{
	mlx_loop_end(mlx->mlx_ptr);
	return (0);
}

void	*ft_destroy_win(t_mlx *mlx)
{
	if (!mlx)
		return (NULL);
	if (mlx->img_ptr)
		mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	if (mlx->win_ptr)
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	if (mlx->mlx_ptr)
		mlx_destroy_display(mlx->mlx_ptr);
	free(mlx->mlx_ptr);
	return (NULL);
}

int	ft_initimage_ptr(t_mlx *mlx)
{
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->img_ptr)
	{
		ft_destroy_win(mlx);
		return (-1);
	}
	mlx->data_addr = mlx_get_data_addr(mlx->img_ptr, &mlx->bits_per_pixel, \
			&mlx->size_line, &mlx->endian);
	if (!mlx->data_addr)
	{
		ft_destroy_win(mlx);
		return (-1);
	}
	return (0);
}

int	ft_initwin_ptr(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	if (!mlx->mlx_ptr)
	{
		ft_destroy_win(mlx);
		return (-1);
	}
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, \
								WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!mlx->win_ptr)
	{
		ft_destroy_win(mlx);
		return (-1);
	}
	return (0);
}

void	*ft_window(t_data *data)
{
	t_mlx	*mlx;

	mlx = data->mlx;
	mlx->mlx_ptr = NULL;
	mlx->win_ptr = NULL;
	mlx->img_ptr = NULL;
	if (ft_initwin_ptr(data->mlx) == -1)
		return (NULL);
	if (ft_initimage_ptr(data->mlx) == -1)
		return (NULL);
	mlx_hook(mlx->win_ptr, DestroyNotify, 0, ft_destroyhandler, mlx);
	mlx_key_hook(mlx->win_ptr, ft_keyhandler, data);
	ft_render(data);
	mlx_loop(mlx->mlx_ptr);
	ft_destroy_win(mlx);
	return (data);
}
