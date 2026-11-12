#include "../include/minirt.h"

/**
 * One line of the key list: the key(s) in blue, what they do in white.
 */
static void	key_row(t_program *prog, int row, char *keys, char *what)
{
	hud_text(prog, row, COLOR_KEY, keys);
	hud_text_value(prog, row, COLOR_TEXT, what);
}

/**
 * What the arrow keys do depends on what is selected.
 */
static char	*arrow_hint(t_program *prog)
{
	if (prog->is_cam || !prog->selected)
		return ("turn the camera");
	if (prog->selected->type == SPHERE)
		return ("diameter");
	if (prog->selected->type == CYLINDER || prog->selected->type == CONE)
		return ("height / width");
	if (prog->selected->type == LIGHT)
		return ("brightness");
	if (prog->selected->type == MESH)
		return ("size / turn");
	return ("-");
}

static void	status_row(t_program *prog)
{
	char	text[64];

	if (prog->final_shown)
	{
		hud_text(prog, HUD_ROWS - 1, COLOR_DIM, "Full quality render (P)");
		return ;
	}
	ft_strlcpy(text, "Samples per pixel: ", sizeof(text));
	hud_append_int(text, prog->scene->sample);
	if (prog->scene->sample >= PROGRESSIVE_MAX_SAMPLES)
		ft_strlcat(text, " (done)", sizeof(text));
	hud_text(prog, HUD_ROWS - 1, COLOR_DIM, text);
}

void	hud_rows(t_program *prog)
{
	char	name[64];
	char	where[64];

	hud_selection(prog, name, where);
	key_row(prog, 0, "miniRT", "               H hide");
	hud_text(prog, 1, COLOR_SELECTED, name);
	hud_text(prog, 2, COLOR_DIM, where);
	key_row(prog, 4, "Click", "select object");
	key_row(prog, 5, "R-click 0", "select camera");
	key_row(prog, 6, "Tab 3", "next object");
	key_row(prog, 7, "W A S D", "move");
	key_row(prog, 8, "PgUp PgDn", "up / down");
	key_row(prog, 9, "Arrows", arrow_hint(prog));
	key_row(prog, 10, "+ -", "move speed");
	key_row(prog, 11, "B", "bump map on / off");
	key_row(prog, 12, "4 5  6 7", "texture scale u, v");
	key_row(prog, 13, "1 2", "pixel size");
	key_row(prog, 14, "X", "reset camera");
	key_row(prog, 15, "P", "full quality render");
	key_row(prog, 16, "Esc", "quit");
	status_row(prog);
}
