#include "minirt.h"

// void	ft_background(t_data *data)
// {
// 	int		x;
// 	int		y;
// 	double	r;
// 	double	g;
// 	char	*dst;

// 	y = 0;
// 	while (y < WIN_HEIGHT)
// 	{
// 		x = 0;
// 		while (x < WIN_WIDTH)
// 		{
// 			r = (double)x / (WIN_WIDTH - 1);
// 			g = (double)y / (WIN_HEIGHT - 1);
// 			dst = data->mlx->data_addr + (y * data->mlx->size_line) + (x * (data->mlx->bits_per_pixel / 8));
// 			*(unsigned int *)dst = ((int)(r * 255.999) << 16) | ((int)(g * 255.999) << 8); // Changed line
// 			x++;
// 		}
// 		y++;
// 	}
// }

void	ft_background(t_data *data)
{
	int	*background;
	int	y;
	int x;
	int	color;
	char	*dst;
	

	background = (int *)(data->mlx->data_addr);
	color = BG_COLOR;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			color = ((int)(((double)x / (WIN_WIDTH - 1) * 255.999)) << 16) + ((int)(((double)y / (WIN_HEIGHT - 1) * 255.999)) << 8);
			dst = data->mlx->data_addr + (y * data->mlx->size_line) + (x * (data->mlx->bits_per_pixel / 8));
			*(unsigned int *)dst = color; //((int)(r * 255.999) << 16) | ((int)(g * 255.999) << 8); // Changed line
			// background[x] = color;
			// background += (y * data->mlx->size_line);
			x++;
		}
		y++;
	}
}

void	ft_render(t_data *data)
{
	int	i;
	// int	j;

	i = 0;
	// if (data->bg == -1)
	// 	ft_startinit(data);
	ft_background(data);
	mlx_put_image_to_window(data->mlx->mlx_ptr, data->mlx->win_ptr,
		data->mlx->img_ptr, 0, 0);
}
