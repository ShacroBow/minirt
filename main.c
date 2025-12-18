#include "minirt.h"

static void cleanup(t_program *prog)
{
	if (prog->mlx.img_ptr)
		mlx_destroy_image(prog->mlx.mlx_ptr, prog->mlx.img_ptr);
	if (prog->mlx.win_ptr)
		mlx_destroy_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr);
#ifdef __linux__
	if (prog->mlx.mlx_ptr)
		mlx_destroy_display(prog->mlx.mlx_ptr);
#endif
	if (prog->mlx.mlx_ptr)
		free(prog->mlx.mlx_ptr);
	free_scene(prog->scene);
}

int close_window(t_program *prog)
{
	cleanup(prog);
	exit(0);
	return (0);
}

int key_hook(int keycode, t_program *prog)
{
	if (keycode == 65307)
		close_window(prog);
	return (0);
}

static void check_filename(const char *filename)
{
	if (!has_extension(filename, ".rt"))
		exit_error("Error: Scene file must have a .rt extension.");
}

int main(int argc, char **argv)
{
	t_program prog;

	ft_bzero(&prog, sizeof(t_program));
	if (argc != 2)
		exit_error("Usage: ./miniRT <scene_file.rt>");
	check_filename(argv[1]);
	parse_scene(argv[1], &prog.scene);
	prog.mlx.mlx_ptr = mlx_init();
	if (!prog.mlx.mlx_ptr)
	{
		cleanup(&prog);
		exit_error("Error: Failed to initialize MiniLibX.");
	}
	prog.mlx.win_ptr = mlx_new_window(prog.mlx.mlx_ptr, WIDTH, HEIGHT, "miniRT");
	prog.mlx.img_ptr = mlx_new_image(prog.mlx.mlx_ptr, WIDTH, HEIGHT);
	prog.mlx.addr = mlx_get_data_addr(prog.mlx.img_ptr, &prog.mlx.bits_per_pixel,
									  &prog.mlx.line_length, &prog.mlx.endian);
	render(&prog);
	mlx_hook(prog.mlx.win_ptr, 17, 0, close_window, &prog);
	mlx_key_hook(prog.mlx.win_ptr, key_hook, &prog);
	mlx_loop(prog.mlx.mlx_ptr);
	cleanup(&prog);
	return (0);
}
