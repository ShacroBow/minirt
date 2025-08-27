#include "minirt.h"

#ifdef __linux__

static void	cleanup_mlx_linux(t_program *prog)
{
	if (prog->mlx.mlx_ptr)
		mlx_destroy_display(prog->mlx.mlx_ptr);
}
#else

static void	cleanup_mlx_linux(t_program *prog)
{
	(void)prog;
}
#endif

void	cleanup(t_program *prog)
{
	if (prog->mlx.img_ptr)
		mlx_destroy_image(prog->mlx.mlx_ptr, prog->mlx.img_ptr);
	if (prog->mlx.win_ptr)
		mlx_destroy_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr);
	cleanup_mlx_linux(prog);
	if (prog->mlx.mlx_ptr)
		free(prog->mlx.mlx_ptr);
	free_scene(prog->scene);
}

static void	check_filename(const char *filename)
{
	if (!has_extension(filename, ".rt"))
		exit_error("Error: Scene file must have a .rt extension.");
}

int	main(int argc, char **argv)
{
	t_scene		*scene;
	t_program	prog;

	if (argc != 2)
		exit_error("Usage: ./miniRT <scene_file.rt>");
	check_filename(argv[1]);
	scene = parse_scene(argv[1]);
	prog.scene = scene;
	prog.mlx.mlx_ptr = mlx_init();
	if (!prog.mlx.mlx_ptr)
		exit_error("Error: Failed to initialize MiniLibX.");
	prog.mlx.win_ptr = mlx_new_window(prog.mlx.mlx_ptr, WIDTH, HEIGHT,
			"miniRT");
	prog.mlx.img_ptr = mlx_new_image(prog.mlx.mlx_ptr, WIDTH, HEIGHT);
	prog.mlx.addr = mlx_get_data_addr(prog.mlx.img_ptr,
			&prog.mlx.bits_per_pixel, &prog.mlx.line_length, &prog.mlx.endian);
	render(&prog);
	mlx_hook(prog.mlx.win_ptr, 17, 0, close_window, &prog);
	mlx_key_hook(prog.mlx.win_ptr, key_hook, &prog);
	mlx_loop(prog.mlx.mlx_ptr);
	cleanup(&prog);
	return (0);
}
