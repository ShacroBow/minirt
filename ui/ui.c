#include "../include/minirt.h"

/**
 * Creates the image behind the help panel and registers every window hook.
 */
void	ui_init(t_program *prog)
{
	int	bpp;
	int	endian;

	prog->is_cam = 1;
	prog->hud = true;
	prog->panel_w = 2 * HUD_PAD + HUD_COLS * FONT_W;
	prog->panel_h = 2 * HUD_PAD + HUD_ROWS * FONT_H;
	prog->panel_img = mlx_new_image(prog->mlx.mlx_ptr, prog->panel_w,
			prog->panel_h);
	if (!prog->panel_img)
		exit_cleanup(prog, "Error: Failed to create image.");
	prog->panel_addr = mlx_get_data_addr(prog->panel_img, &bpp,
			&prog->panel_line, &endian);
	mlx_hook(prog->mlx.win_ptr, 17, 0, close_window, prog);
	mlx_key_hook(prog->mlx.win_ptr, key_hook, prog);
	mlx_mouse_hook(prog->mlx.win_ptr, mouse_hook, prog);
	mlx_expose_hook(prog->mlx.win_ptr, expose_hook, prog);
	mlx_loop_hook(prog->mlx.mlx_ptr, progressive_hook, prog);
}

/**
 * Shows the rendered image, with the help panel on top when it is on.
 */
void	display_frame(t_program *prog)
{
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
		prog->mlx.img_ptr, 0, 0);
	if (prog->hud && prog->panel_img)
		hud_draw(prog);
}

/**
 * obj NULL selects the camera.
 */
void	select_object(t_program *prog, t_object *obj)
{
	prog->selected = obj;
	prog->is_cam = (obj == NULL);
	if (obj)
		printf("selected %s\n", obj->name);
	else
		printf("selected camera\n");
	display_frame(prog);
}

static bool	in_panel(t_program *prog, int x, int y)
{
	return (prog->hud && x >= HUD_X && x < HUD_X + prog->panel_w
		&& y >= HUD_Y && y < HUD_Y + prog->panel_h);
}

/**
 * Left click selects the object or light marker under the mouse (the
 * camera if there is none), right / middle click selects the camera.
 */
int	mouse_hook(int button, int x, int y, void *param)
{
	t_program	*prog;
	t_object	*obj;

	prog = (t_program *)param;
	if (button < 1 || button > 3 || in_panel(prog, x, y))
		return (0);
	obj = NULL;
	if (button == 1)
		obj = pick_light(prog, x, y);
	if (button == 1 && !obj)
		obj = pick_object(prog, x, y);
	select_object(prog, obj);
	return (0);
}
