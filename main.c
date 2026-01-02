#include "include/minirt.h"

# define TAB_WIDTH (WIDTH / 3 + WIDTH % 3)

int	close_window(t_program *prog)
{
	long	end_time;

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
	if (k == XK_Return)
		return (true);
	return (false);
}

void	put_args(t_program *prog, t_object *obj, t_object *selected_obj, int selected_arg, int x_offset, int y_offset)
{
	int		i;
	char	*arg;

	(void)selected_arg;
	(void)selected_obj;
	i = 0;
	while (i < 5) // ARGS_COUNT
	{
		arg = ft_itoa(i);
		if (!arg)
			erorr(prog->scene, NULL, "Error: Tab tab allocation failed.");
		if (i == selected_arg && obj == selected_obj)
		{
			mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
				x_offset + 50, y_offset, 0xFFFFFF, "[");
			x_offset += 10;
		}
		mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
			x_offset + 50, y_offset, 0xFFFFFF, arg);
		x_offset += 10 * ft_strlen(arg);
		if (i == selected_arg && obj == selected_obj)
		{
			mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
				x_offset + 50, y_offset, 0xFFFFFF, "]");
		}
		x_offset += 10 * ft_strlen(arg);
		i++;
	}
}

void	display_tab(t_program *prog, t_object *selected_obj, int selected_arg)
{
	int		x_offset;
	int		y_offset;
	int		i;
	int		j;
	t_object	*obj;

	x_offset = 2 * TAB_WIDTH;
	y_offset = 30;
	j = 0;
	i = 0;
	obj = prog->scene->objects;
	if (prog->mlx.tab_active)
	{
		mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr, prog->mlx.tab_img_ptr, WIDTH - TAB_WIDTH, 0);
		mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
			x_offset, 10, 0xFFFFFF, "[ACTIVE]");
	}
	else
		mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr, prog->mlx.tab_img_ptr, WIDTH - TAB_WIDTH, 0);
	while (obj)
	{
		if (obj == selected_obj)
		{
			mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
				x_offset, y_offset, 0xFFFFFF, ">");
			x_offset += 10;
		}
		mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr, \
			x_offset, y_offset, 0xFFFFFF, obj->name);
		put_args(prog, obj, selected_obj, selected_arg, x_offset, y_offset);
		if (obj == selected_obj)
			x_offset -= 10;
		y_offset += 20;
		obj = obj->next;
	}
}

void	tab_key_hooks(int keycode, t_program *prog, \
	t_object **selected_obj, int *is_cam)
{
	static int	selected_arg = 0;

	if (keycode == KEY_TAB)
	{
		prog->mlx.tab_active = false;
		display_tab(prog, *selected_obj, 0);
	}
	if (keycode == KEY_DOWN)
	{
		selected_arg = 0;
		*is_cam = 0;
		if (!*selected_obj)
			*selected_obj = prog->scene->objects;
		else
			*selected_obj = (*selected_obj)->next;
		if (!*selected_obj)
			*selected_obj = prog->scene->objects;
		if (!*selected_obj)
			*is_cam = 1;
		printf("Selected object: %s\n", (*selected_obj)->name);
		display_tab(prog, *selected_obj, 0);
	}
	else if (keycode == KEY_UP)
	{
		selected_arg = 0;
		*is_cam = 0;
		if (!*selected_obj)
			*selected_obj = prog->scene->objects;
		else
			*selected_obj = (*selected_obj)->prev;
		if (!*selected_obj)
			*selected_obj = prog->scene->objects_last;
		if (!*selected_obj)
			*is_cam = 1;
		printf("Selected object: %s\n", (*selected_obj)->name);
		display_tab(prog, *selected_obj, 0);		
	}
	if (*selected_obj && keycode == KEY_RIGHT)
	{
		// if (selected_arg < (*selected_obj)->args_acount)
		selected_arg++;
		display_tab(prog, *selected_obj, selected_arg);
	}
	if (*selected_obj && keycode == KEY_LEFT)
	{
		selected_arg--;
		display_tab(prog, *selected_obj, selected_arg);
	}
	else if (keycode == KEY_ESC)
		close_window(prog);
}

void	scene_key_hooks(int keycode, t_program *prog, \
	t_object **object, int *is_cam)
{
	if (keycode == KEY_ESC)
		close_window(prog);
	else if (keycode == KEY_P)
	{
		prog->aa_enabled = 1;
		render(prog);
		prog->aa_enabled = 0;
		return ;
	}
	else
		camera_handle_key(keycode, prog, object, is_cam);
	if ((keycode == KEY_0 || keycode == KEY_3))
	{
		display_tab(prog, *object, 0);
		return ;
	}
	if (keycode == KEY_TAB)
	{
		prog->mlx.tab_active = true;
		display_tab(prog, *object, 0);
	}
	if (key_changes_scene(keycode))
		render(prog);
}

int	key_hook(int keycode, t_program *prog)
{
	static t_object		*object = NULL;
	static int			is_cam = 1;

	if (!prog->mlx.tab_active)
		scene_key_hooks(keycode, prog, &object, &is_cam);
	else
		tab_key_hooks(keycode, prog, &object, &is_cam);
	return (0);
}

void	clear_tab_img(t_mlx_data *mlx)
{
	int		x;
	int		y;
	char	*dst;

	x = 0;
	y = 0;
	while (y < HEIGHT)
	{
		while (x < TAB_WIDTH)
		{
			dst = mlx->tab_img_addr + y * mlx->tab_line_length + \
				x * (mlx->bits_per_pixel / 8);
			*(unsigned int *)dst = 0;
			x++;
		}
		y++;
	}
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
	prog->mlx.img_ptr = mlx_new_image(prog->mlx.mlx_ptr, WIDTH - TAB_WIDTH, HEIGHT);
	if (!prog->mlx.img_ptr)
		exit_cleanup(prog, "Error: Failed to create image.");
	prog->mlx.tab_img_ptr = mlx_new_image(prog->mlx.mlx_ptr,
		TAB_WIDTH, HEIGHT);
	if (!prog->mlx.tab_img_ptr)
		exit_cleanup(prog, "Error: Failed to create tab image.");
	prog->mlx.addr = mlx_get_data_addr(prog->mlx.img_ptr, \
		&prog->mlx.bits_per_pixel, &prog->mlx.line_length, &prog->mlx.endian);
	if (!prog->mlx.addr)
		exit_cleanup(prog, "Error: Failed to get image data address.");
	prog->mlx.tab_img_addr = mlx_get_data_addr(prog->mlx.tab_img_ptr,
		&prog->mlx.bits_per_pixel, &prog->mlx.tab_line_length, &prog->mlx.endian);
	if (!prog->mlx.tab_img_addr)
		exit_cleanup(prog, "Error: Failed to get tab image data address.");
	prog->mlx.tab_active = false;
	clear_tab_img(&prog->mlx);
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
	print_objects_status(prog.scene);
	prog.default_camera = prog.scene->camera;
	init_mlx(&prog);
	prog.program_start_time = get_time_ms();
	mlx_hook(prog.mlx.win_ptr, 17, 0, close_window, &prog);
	mlx_key_hook(prog.mlx.win_ptr, key_hook, &prog);
	display_tab(&prog, NULL, 0);
	mlx_loop(prog.mlx.mlx_ptr);
	cleanup(&prog);
	return (0);
}
