#include "../include/minirt.h"

void	exit_cleanup(t_program *prog, const char *message)
{
	cleanup(prog);
	if (message)
	{
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

void	erorr(t_scene *scene, void *ptr, const char *message)
{
	if (ptr)
		free(ptr);
	free_scene(scene);
	if (message)
	{
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

void	cleanup(t_program *prog)
{
	if (prog->mlx.img_ptr)
		mlx_destroy_image(prog->mlx.mlx_ptr, prog->mlx.img_ptr);
	if (prog->mlx.win_ptr)
		mlx_destroy_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr);
	if (prog->mlx.mlx_ptr)
		mlx_destroy_display(prog->mlx.mlx_ptr);
	if (prog->mlx.mlx_ptr)
		free(prog->mlx.mlx_ptr);
	free_scene(prog->scene);
}
