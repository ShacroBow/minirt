#include "../include/minirt.h"

/**
 * Dimmed, slightly blue version of a pixel: the see-through panel color.
 */
static unsigned int	darken(unsigned int c)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = ((c >> 16) & 255) * 3 / 10 + 8;
	g = ((c >> 8) & 255) * 3 / 10 + 10;
	b = (c & 255) * 3 / 10 + 20;
	return ((r << 16) | (g << 8) | b);
}

/**
 * Copies the rendered pixels under the panel, darkened, into the panel
 * image. The rendered image itself is never touched.
 */
static void	panel_background(t_program *prog)
{
	unsigned int	*src;
	unsigned int	*dst;
	int				x;
	int				y;

	y = 0;
	while (y < prog->panel_h && y + HUD_Y < HEIGHT)
	{
		src = (unsigned int *)(prog->mlx.addr
				+ (ptrdiff_t)(y + HUD_Y) * prog->mlx.line_length) + HUD_X;
		dst = (unsigned int *)(prog->panel_addr
				+ (ptrdiff_t)y * prog->panel_line);
		x = -1;
		while (++x < prog->panel_w && x + HUD_X < WIDTH)
			dst[x] = darken(src[x]);
		y++;
	}
}

static void	world_label(t_program *prog, t_object *obj, char *text,
				int color)
{
	int	p[2];

	if (!project_point(&prog->scene->camera, object_center(obj),
			&p[0], &p[1]))
		return ;
	p[0] -= (int)ft_strlen(text) * HUD_CHAR_W / 2;
	if (p[0] < HUD_X + prog->panel_w && p[1] < HUD_Y + prog->panel_h + FONT_H)
		return ;
	mlx_string_put(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
		p[0], p[1] + HUD_ASCENT / 2, color, text);
}

/**
 * Light markers ("L1", "L2", ...) and the name of the selected object,
 * drawn where they are in the scene (only a few letters, so mlx_string_put
 * is fine here).
 */
static void	world_labels(t_program *prog)
{
	t_object	*cur;
	char		text[64];
	int			color;

	cur = prog->scene->objects;
	while (cur)
	{
		color = COLOR_MARKER;
		if (cur == prog->selected)
			color = COLOR_SELECTED;
		if (cur->type == LIGHT)
		{
			ft_strlcpy(text, "L", sizeof(text));
			hud_append_int(text, light_number(prog->scene, cur));
			world_label(prog, cur, text, color);
		}
		else if (cur == prog->selected)
		{
			ft_strlcpy(text, "[ ", sizeof(text));
			ft_strlcat(text, cur->name, sizeof(text) - 2);
			ft_strlcat(text, " ]", sizeof(text));
			world_label(prog, cur, text, color);
		}
		cur = cur->next;
	}
}

/**
 * The panel is one image with its text already drawn in: one draw call,
 * instead of one per letter with mlx_string_put.
 */
void	hud_draw(t_program *prog)
{
	setup_camera(&prog->scene->camera, (double)WIDTH / (double)HEIGHT);
	panel_background(prog);
	hud_rows(prog);
	mlx_put_image_to_window(prog->mlx.mlx_ptr, prog->mlx.win_ptr,
		prog->panel_img, HUD_X, HUD_Y);
	world_labels(prog);
}
