#include "minirt.h"

int close_window(t_program *prog)
{
	cleanup(prog);
	exit(0);
	return (0);
}

static void	move_camera(t_camera *cam, t_vec3 direction, double speed)
{
	t_vec3	move;

	move = vec_mult(direction, speed);
	cam->center = vec_add(cam->center, move);
}

static void	rotate_camera(t_camera *cam, double pitch, double yaw)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	double	cos_pitch;
	double	sin_pitch;
	double	cos_yaw;
	double	sin_yaw;

	forward = cam->normal;
	if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		right = vec_normalize(vec_cross(forward, (t_vec3){1, 0, 0}));
	else
		right = vec_normalize(vec_cross(forward, (t_vec3){0, 1, 0}));
	up = vec_cross(right, forward);
	cos_pitch = cos(pitch);
	sin_pitch = sin(pitch);
	cos_yaw = cos(yaw);
	sin_yaw = sin(yaw);
	if (fabs(pitch) > EPSILON)
	{
		forward = vec_add(vec_mult(forward, cos_pitch),
				vec_mult(up, sin_pitch));
		forward = vec_normalize(forward);
	}
	if (fabs(yaw) > EPSILON)
	{
		forward = vec_add(vec_mult(forward, cos_yaw),
				vec_mult(right, sin_yaw));
		forward = vec_normalize(forward);
	}
	cam->normal = forward;
}

int key_hook(int keycode, t_program *prog)
{
	t_camera	*cam;
	t_vec3		forward;
	t_vec3		right;
	t_vec3		forward_xz;

	cam = &prog->scene->camera;
	forward = cam->normal;
	if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		right = vec_normalize(vec_cross(forward, (t_vec3){1, 0, 0}));
	else
		right = vec_normalize(vec_cross(forward, (t_vec3){0, 1, 0}));
	forward_xz = vec_normalize((t_vec3){forward.x, 0, forward.z});
	if (keycode == KEY_ESC)
		close_window(prog);
	else if (keycode == KEY_W)
		move_camera(cam, forward_xz, prog->move_speed);
	else if (keycode == KEY_S)
		move_camera(cam, forward_xz, -prog->move_speed);
	else if (keycode == KEY_D)
		move_camera(cam, right, -prog->move_speed);
	else if (keycode == KEY_A)
		move_camera(cam, right, prog->move_speed);
	else if (keycode == KEY_PG_UP)
		move_camera(cam, (t_vec3){0, 1, 0}, prog->move_speed * 2.0);
	else if (keycode == KEY_PG_DOWN)
		move_camera(cam, (t_vec3){0, -1, 0}, prog->move_speed * 2.0);
	else if (keycode == KEY_LEFT)
		rotate_camera(cam, 0, ROT_SPEED);
	else if (keycode == KEY_RIGHT)
		rotate_camera(cam, 0, -ROT_SPEED);
	else if (keycode == KEY_UP)
		rotate_camera(cam, ROT_SPEED, 0);
	else if (keycode == KEY_DOWN)
		rotate_camera(cam, -ROT_SPEED, 0);
	else if (keycode == KEY_PLUS)
		prog->move_speed += SPEED_INCREMENT;
	else if (keycode == KEY_MINUS && prog->move_speed > SPEED_INCREMENT)
		prog->move_speed -= SPEED_INCREMENT;
	else if (keycode == KEY_X)
		*cam = prog->default_camera;
	else if (keycode == KEY_P)
	{
		prog->aa_enabled = 1;
		render(prog);
		prog->aa_enabled = 0;
		return (0);
	}
	else if (keycode == KEY_O)
		prog->aa_enabled = !prog->aa_enabled;
	render(prog);
	return (0);
}

static void	init_mlx(t_program *prog)
{
	prog->mlx.mlx_ptr = mlx_init();
	if (!prog->mlx.mlx_ptr)
		exit_error_with_cleanup(prog, "Error: Failed to initialize MiniLibX.");
	prog->mlx.win_ptr = mlx_new_window(prog->mlx.mlx_ptr, WIDTH, HEIGHT, WIN_TITLE);
	if (!prog->mlx.win_ptr)
		exit_error_with_cleanup(prog, "Error: Failed to create window.");
	prog->mlx.img_ptr = mlx_new_image(prog->mlx.mlx_ptr, WIDTH, HEIGHT);
	if (!prog->mlx.img_ptr)
		exit_error_with_cleanup(prog, "Error: Failed to create image.");
	prog->mlx.addr = mlx_get_data_addr(prog->mlx.img_ptr, &prog->mlx.bits_per_pixel,
			&prog->mlx.line_length, &prog->mlx.endian);
	if (!prog->mlx.addr)
		exit_error_with_cleanup(prog, "Error: Failed to get image data address.");
}

int main(int argc, char **argv)
{
	t_program prog;

	ft_bzero(&prog, sizeof(t_program));
	if (argc != 2)
		exit_error_with_cleanup(&prog, "Usage: ./miniRT <scene_file.rt>");
	if (!has_extension(argv[1], ".rt"))
		exit_error_with_cleanup(&prog, "Error: Scene file must have a .rt extension.");
	prog.scene_file = argv[1];
	prog.move_speed = MOVE_SPEED_BASE;
	prog.aa_enabled = ENABLE_AA;
	parse_scene(prog.scene_file, &prog.scene);
	prog.default_camera = prog.scene->camera;
	init_mlx(&prog);
	render(&prog);
	mlx_hook(prog.mlx.win_ptr, 17, 0, close_window, &prog);
	mlx_key_hook(prog.mlx.win_ptr, key_hook, &prog);
	mlx_loop(prog.mlx.mlx_ptr);
	cleanup(&prog);
	return (0);
}
