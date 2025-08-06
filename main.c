#include "minirt.h"

int	close_window(t_program *prog)
{
	free_scene(prog->scene);
	mlx_destroy_image(prog->mlx.mlx_ptr, prog->mlx.img_ptr);
	mlx_destroy_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr);
	exit(0);
	return (0);
}

int	key_hook(int keycode, t_program *prog)
{
	if (keycode == 65307) // ESC key for Linux X11
	{
		close_window(prog);
	}
	return (0);
}

static void	check_filename(const char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4 || ft_strncmp(filename + len - 3, ".rt", 4)!= 0)
		exit_error("Error: Scene file must have a.rt extension.");
}

int	main(int argc, char **argv)
{
	t_program	prog;
	t_scene		*scene;

	if (argc!= 2)
		exit_error("Usage:./miniRT <scene_file.rt>");
	check_filename(argv[1]);
	scene = parse_scene(argv[1]);
	prog.scene = scene;
	prog.mlx.mlx_ptr = mlx_init();
	if (!prog.mlx.mlx_ptr)
		exit_error("Error: Failed to initialize MiniLibX.");
	prog.mlx.win_ptr = mlx_new_window(prog.mlx.mlx_ptr, WIDTH, HEIGHT, "miniRT");
	prog.mlx.img_ptr = mlx_new_image(prog.mlx.mlx_ptr, WIDTH, HEIGHT);
	prog.mlx.addr = mlx_get_data_addr(prog.mlx.img_ptr, &prog.mlx.bits_per_pixel,
			&prog.mlx.line_length, &prog.mlx.endian);
	printf("Rendering scene...\n");
	render(&prog);
	printf("Rendering complete.\n");
	mlx_put_image_to_window(prog.mlx.mlx_ptr, prog.mlx.win_ptr,
		prog.mlx.img_ptr, 0, 0);
	mlx_hook(prog.mlx.win_ptr, 17, 0, close_window, &prog);
	mlx_key_hook(prog.mlx.win_ptr, key_hook, &prog);
	mlx_loop(prog.mlx.mlx_ptr);
	return (0);
}