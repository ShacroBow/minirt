#include "include/minirt.h"

int	close_window(t_program *prog)
{
	long	end_time;

	end_time = get_time_ms();
	printf("\nProgram lifetime: %ldms\n", end_time - prog->program_start_time);
	cleanup(prog);
	exit(0);
	return (0);
}

int	key_hook(int keycode, t_program *prog)
{
	static t_object		*object = NULL;
	static int			is_cam = 1;

	if (keycode == KEY_ESC)
		close_window(prog);
	else if (keycode == KEY_P)
	{
		prog->aa_enabled = 1;
		render(prog);
		prog->aa_enabled = 0;
		return (0);
	}
	else
		camera_handle_key(keycode, prog, &object, &is_cam);
	if ((keycode == KEY_0 || keycode == KEY_3))
		return (0);
	render(prog);
	return (0);
}

static void	init_mlx(t_program *prog)
{
	prog->mlx.mlx_ptr = mlx_init();
	if (!prog->mlx.mlx_ptr)
		exit_cleanup(prog, "Error: Failed to initialize MiniLibX.");
	prog->mlx.win_ptr = mlx_new_window(prog->mlx.mlx_ptr, WIDTH, HEIGHT, \
															WIN_TITLE);
	if (!prog->mlx.win_ptr)
		exit_cleanup(prog, "Error: Failed to create window.");
	prog->mlx.img_ptr = mlx_new_image(prog->mlx.mlx_ptr, WIDTH, HEIGHT);
	if (!prog->mlx.img_ptr)
		exit_cleanup(prog, "Error: Failed to create image.");
	prog->mlx.addr = mlx_get_data_addr(prog->mlx.img_ptr, \
		&prog->mlx.bits_per_pixel, &prog->mlx.line_length, &prog->mlx.endian);
	if (!prog->mlx.addr)
		exit_cleanup(prog, "Error: Failed to get image data address.");
}

int	main(int argc, char **argv)
{
	t_program	prog;

	ft_bzero(&prog, sizeof(t_program));
	if (argc != 2)
		exit_cleanup(&prog, "Usage: ./miniRT <scene_file.rt>");
	if (!has_extension(argv[1], ".rt"))
		exit_cleanup(&prog, "Error: Scene file must have a .rt extension.");
	prog.scene_file = argv[1];
	prog.move_speed = MOVE_SPEED_BASE;
	prog.aa_enabled = ENABLE_AA;
	prog.pixel_step = ENABLE_PIXEL_STEP;
	parse_scene(prog.scene_file, &prog.scene);
	prog.default_camera = prog.scene->camera;
	init_mlx(&prog);
	prog.program_start_time = get_time_ms();
	mlx_hook(prog.mlx.win_ptr, 17, 0, close_window, &prog);
	mlx_key_hook(prog.mlx.win_ptr, key_hook, &prog);
	mlx_loop(prog.mlx.mlx_ptr);
	cleanup(&prog);
	return (0);
}
