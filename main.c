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

static void	select_element(int *is_cam, t_object **object, int keycode, t_program *prog)
{
	if (keycode == KEY_0)
		*is_cam = 1;
	else if (keycode == KEY_3)
	{
		*is_cam = 0;
		if (!*object)
			*object = prog->scene->objects;
		else
			*object = (*object)->next;
		if (!*object)
			*object = prog->scene->objects;
		if (!*object)
			*is_cam = 1;
	}
	if (DEBUG)
	{
		if (*is_cam)
			printf("selected cam\n");
		else if (*object && (*object)->type == SPHERE)
			printf("selected SPHERE\n");
		else if (*object && (*object)->type == CYLINDER)
			printf("selected CYLINDER\n");
		else if (*object && (*object)->type == PLANE)
			printf("selected PLANE\n");
	}
}

static void	move_object(t_object *object, t_vec3 direction, double speed)
{
	t_vec3	move;

	move = vec_mult(direction, speed);
	if (object->type == SPHERE)
		((t_sphere *)object->shape_data)->center = vec_add(((t_sphere *)object->shape_data)->center, move);
	else if (object->type == CYLINDER)
		((t_cylinder *)object->shape_data)->center = vec_add(((t_sphere *)object->shape_data)->center, move);
	else if (object->type == PLANE)
		((t_plane *)object->shape_data)->point = vec_add(((t_sphere *)object->shape_data)->center, move);
}

static void	move_element_camera(t_program* prog, int keycode)
{
	t_vec3	forward;
	t_vec3	forward_xz;
	t_vec3	right;

	forward = prog->scene->camera.normal;
	if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		right = vec_normalize(vec_cross(forward, (t_vec3){1, 0, 0}));
	else
		right = vec_normalize(vec_cross(forward, (t_vec3){0, 1, 0}));
	forward_xz = vec_normalize((t_vec3){forward.x, 0, forward.z});
	if (keycode == KEY_W)
		move_camera(&prog->scene->camera, forward_xz, prog->move_speed);
	else if (keycode == KEY_S)
		move_camera(&prog->scene->camera, forward_xz, -prog->move_speed);
	else if (keycode == KEY_D)
		move_camera(&prog->scene->camera, right, -prog->move_speed);
	else if (keycode == KEY_A)
		move_camera(&prog->scene->camera, right, prog->move_speed);
	else if (keycode == KEY_PG_UP)
		move_camera(&prog->scene->camera, (t_vec3){0, 1, 0}, prog->move_speed * 2.0);
	else if (keycode == KEY_PG_DOWN)
		move_camera(&prog->scene->camera, (t_vec3){0, -1, 0}, prog->move_speed * 2.0);
}

static void	move_element_object(t_program *prog, int keycode, t_object *object)
{
	t_vec3	forward;
	t_vec3	forward_xz;
	t_vec3	right;

	forward = prog->scene->camera.normal;
	if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		right = vec_normalize(vec_cross(forward, (t_vec3){1, 0, 0}));
	else
		right = vec_normalize(vec_cross(forward, (t_vec3){0, 1, 0}));
	forward_xz = vec_normalize((t_vec3){forward.x, 0, forward.z});
	if (keycode == KEY_W)
		move_object(object, forward_xz, prog->move_speed);
	if (keycode == KEY_S)
		move_object(object, forward_xz, -prog->move_speed);
	if (keycode == KEY_D)
		move_object(object, right, -prog->move_speed);
	if (keycode == KEY_A)
		move_object(object, right, prog->move_speed);
	if (keycode == KEY_PG_UP)
		move_object(object, (t_vec3){0, 1, 0}, prog->move_speed * 2.0);
	if (keycode == KEY_PG_DOWN)
		move_object(object, (t_vec3){0, -1, 0}, prog->move_speed * 2.0);
}

static void move_element(int is_cam, t_object *object, t_program *prog, int keycode)
{
	if (is_cam)
		move_element_camera(prog, keycode);
	else
		move_element_object(prog, keycode, object);
}

int key_hook(int keycode, t_program *prog)
{
	t_camera			*cam;
	static t_object		*object = NULL;
	static int			is_cam = 1;

	cam = &prog->scene->camera;
	if (keycode == KEY_ESC)
		close_window(prog);
	else if (keycode >= KEY_0 && keycode <= KEY_9)
		select_element(&is_cam, &object, keycode, prog);
	else if (keycode == KEY_W)
		move_element(is_cam, object, prog, keycode);
	else if (keycode == KEY_S)
		move_element(is_cam, object, prog, keycode);
	else if (keycode == KEY_D)
		move_element(is_cam, object, prog, keycode);
	else if (keycode == KEY_A)
		move_element(is_cam, object, prog, keycode);
	else if (keycode == KEY_PG_UP)
		move_element(is_cam, object, prog, keycode);
	else if (keycode == KEY_PG_DOWN)
		move_element(is_cam, object, prog, keycode);
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
