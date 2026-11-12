#include "../include/minirt.h"

/**
 * dst covered by src with coverage a (0 = none, 3 = full).
 */
static unsigned int	mix(unsigned int dst, unsigned int src, unsigned int a)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = (((dst >> 16) & 255) * (3 - a) + ((src >> 16) & 255) * a) / 3;
	g = (((dst >> 8) & 255) * (3 - a) + ((src >> 8) & 255) * a) / 3;
	b = ((dst & 255) * (3 - a) + (src & 255) * a) / 3;
	return ((r << 16) | (g << 8) | b);
}

static void	draw_glyph(t_program *prog, const t_pen *pen, char c)
{
	unsigned int	*row;
	unsigned int	bits;
	unsigned int	a;
	int				x;
	int				y;

	y = -1;
	while (++y < FONT_H && pen->y + y < prog->panel_h)
	{
		bits = font_row(c, y);
		row = (unsigned int *)(prog->panel_addr
				+ (ptrdiff_t)(pen->y + y) * prog->panel_line);
		x = -1;
		while (++x < FONT_W && pen->x + x < prog->panel_w)
		{
			a = (bits >> (2 * x)) & 3;
			if (a)
				row[pen->x + x] = mix(row[pen->x + x], pen->color, a);
		}
	}
}

static void	draw_string(t_program *prog, t_pen pen, const char *s)
{
	while (*s)
	{
		if (*s != ' ')
			draw_glyph(prog, &pen, *s);
		pen.x += FONT_W;
		s++;
	}
}

/**
 * Writes text into the help panel image, in the first column of a row.
 */
void	hud_text(t_program *prog, int row, int color, char *text)
{
	draw_string(prog, (t_pen){HUD_PAD, HUD_PAD + row * FONT_H, color},
		text);
}

/**
 * Same, in the second column (after the keys).
 */
void	hud_text_value(t_program *prog, int row, int color, char *text)
{
	draw_string(prog, (t_pen){HUD_PAD + HUD_KEY_COLS * FONT_W,
		HUD_PAD + row * FONT_H, color}, text);
}
