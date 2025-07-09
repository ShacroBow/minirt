#ifndef MINIRT_H
# define MINIRT_H
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <limits.h>
# include "libft/libft.h"
# include <mlx.h>
# include <X11/Xutil.h>

# include <stdio.h>
# define BUFFER_SIZE 1000
# define BG_COLOR 0x09090B
# define WIN_WIDTH (1920 / 2)
# define WIN_HEIGHT (1080 / 2)
# define DEF_LINECOLOR 0xffffff
# define WIN_TITLE "Khaled's FdF"
# define PADDED_WIDTH 1920
# define PADDED_HEIGHT 1080
# define KEY_ESC XK_Escape //65307
# define KEY_LEFT XK_Left //65361
# define KEY_UP XK_Up //65362
# define KEY_RIGHT XK_Right //65363
# define KEY_DOWN XK_Down //65364
# define KEY_PG_UP XK_Page_Up //65365
# define KEY_PG_DOWN XK_Page_Down //65366
# define KEY_PLUS XK_equal //61
# define KEY_MINUS XK_minus //45
# define KEY_SPACE XK_space //32
# define KEY_P XK_p //70
# define KEY_A XK_a //61
# define KEY_W XK_w //77
# define KEY_D XK_d //64
# define KEY_S XK_s //73
# define KEY_X XK_x //78

typedef struct s_mlx
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*data_addr;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}			t_mlx;

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
	int	color;
}			t_point;

typedef struct s_data
{
	t_mlx	*mlx;
	t_point	***map;
	int		bg;
}			t_data;

//window
void	*ft_window(t_data *data);
int		ft_destroyhandler(t_mlx *mlx);
void	*ft_destroy_win(t_mlx *mlx);
int		ft_initimage_ptr(t_mlx *mlx);
int		ft_initwin_ptr(t_mlx *mlx);
//window
void	ft_render(t_data *data);

#endif