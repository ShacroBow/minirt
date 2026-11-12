#include "include/minirt.h"

int	close_window(void *param)
{
	t_program	*prog;
	long		end_time;

	prog = (t_program *)param;
	end_time = get_time_ms();
	printf("\nProgram lifetime: %ldms\n", end_time - prog->program_start_time);
	cleanup(prog);
	exit(0);
	return (0);
}

static bool	key_changes_scene(int k)
{
	if (k == KEY_W || k == KEY_S || k == KEY_D || k == KEY_A)
		return (true);
	if (k == KEY_LEFT || k == KEY_RIGHT || k == KEY_UP || k == KEY_DOWN)
		return (true);
	if (k == KEY_X || k == KEY_B || k == KEY_PG_UP || k == KEY_PG_DOWN)
		return (true);
	if (k == KEY_4 || k == KEY_5 || k == KEY_6 || k == KEY_7)
		return (true);
	if (k == KEY_O || k == KEY_1 || k == KEY_2)
		return (true);
	if (k == KEY_ENTER)
		return (true);
	return (false);
}

int	key_hook(int keycode, void *param)
{
	t_program	*prog;

	prog = (t_program *)param;
	if (keycode == KEY_ESC)
		close_window(prog);
	else if (keycode == KEY_P)
		render_final(prog);
	else if (keycode == KEY_H)
		prog->hud = !prog->hud;
	else
		camera_handle_key(keycode, prog, &prog->selected, &prog->is_cam);
	if (key_changes_scene(keycode))
		progressive_reset(prog);
	else
		display_frame(prog);
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
	init_threading(&prog);
	if (argc != 2)
		exit_cleanup(&prog, "Usage: ./miniRT <scene_file.rt>");
	if (!has_extension(argv[1], ".rt"))
		exit_cleanup(&prog, "Error: Scene file must have a .rt extension.");
	prog.scene_file = argv[1];
	prog.move_speed = MOVE_SPEED_BASE;
	prog.aa_enabled = ENABLE_AA;
	prog.pixel_step = ENABLE_PIXEL_STEP;
	parse_scene(prog.scene_file, &prog.scene);
	print_objects_status(prog.scene);
	prog.default_camera = prog.scene->camera;
	init_mlx(&prog);
	prog.program_start_time = get_time_ms();
	progressive_init(&prog);
	ui_init(&prog);
	mlx_loop(prog.mlx.mlx_ptr);
	cleanup(&prog);
	return (0);
}
