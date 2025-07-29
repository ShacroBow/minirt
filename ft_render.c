#include "minirt.h"
#include "vec3.h"
#include "ray.h"

static int	color_to_int(t_color pixel_color)
{
	// ... (same as before)
	int r = (int)(255.999 * pixel_color.x);
	int g = (int)(255.999 * pixel_color.y);
	int b = (int)(255.999 * pixel_color.z);
	return ((r << 16) | (g << 8) | b);
}

// New function to calculate sky color based on ray direction


// New main rendering loop that shoots rays
void	render_scene(t_data *data)
{
    char		*pixel_addr;
    int			y, x;
    double		viewport_height = 2.0;
    double		viewport_width = viewport_height * ((double)WIN_WIDTH / WIN_HEIGHT);
    t_point3	camera_center = {0, 0, 0};
    t_vec3		ray_direction;
    t_ray		r;
    t_color		c;

    y = 0;
    while (y < WIN_HEIGHT)
    {
        x = 0;
        while (x < WIN_WIDTH)
        {
            // Calculate the direction for the ray for pixel (x,y)
            ray_direction.x = (double)x / (WIN_WIDTH - 1) * viewport_width - (viewport_width / 2);
            // ray_direction.y = (double)y / (WIN_HEIGHT - 1) * viewport_height - (viewport_height / 2);
			ray_direction.y = (viewport_height / 2) - (double)y / (WIN_HEIGHT - 1) * viewport_height;
            ray_direction.z = -1.0; // Point into the screen
            r.origin = camera_center;
            r.direction = ray_direction;

            c = ray_color(r);

            pixel_addr = data->mlx->data_addr + (y * data->mlx->size_line)
                + (x * (data->mlx->bits_per_pixel / 8));
            *(unsigned int *)pixel_addr = color_to_int(c);
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
	// ft_background(data);
	// printf("\r\n");
	// draw_checkerboard(data);
    render_scene(data);
	mlx_put_image_to_window(data->mlx->mlx_ptr, data->mlx->win_ptr,
		data->mlx->img_ptr, 0, 0);
}
