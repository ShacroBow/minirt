#include "../include/minirt.h"

/**
 * Appends n in decimal to the string in buf.
 */
void	hud_append_int(char *buf, long n)
{
	char	tmp[24];
	int		i;
	bool	neg;

	buf += ft_strlen(buf);
	neg = (n < 0);
	i = 0;
	tmp[i++] = (char)('0' + labs(n % 10));
	n /= 10;
	while (n != 0 && i < 20)
	{
		tmp[i++] = (char)('0' + labs(n % 10));
		n /= 10;
	}
	if (neg)
		tmp[i++] = '-';
	while (i > 0)
	{
		*buf = tmp[i - 1];
		buf++;
		i--;
	}
	*buf = '\0';
}

/**
 * Appends v with one decimal ("-12.3") to the string in buf.
 */
void	hud_append_num(char *buf, double v)
{
	long	tenths;
	char	digit[2];

	if (!isfinite(v) || fabs(v) > 1e12)
	{
		ft_strlcat(buf, "?", 64);
		return ;
	}
	tenths = (long)(fabs(v) * 10.0 + 0.5);
	if (v < 0.0 && tenths != 0)
		ft_strlcat(buf, "-", 64);
	hud_append_int(buf, tenths / 10);
	digit[0] = (char)('0' + tenths % 10);
	digit[1] = '\0';
	ft_strlcat(buf, ".", 64);
	ft_strlcat(buf, digit, 64);
}

/**
 * Position of the object (as "position: x, y, z") in where.
 */
static void	describe_position(t_vec3 p, char *where)
{
	ft_strlcpy(where, "position: ", 64);
	hud_append_num(where, p.x);
	ft_strlcat(where, ", ", 64);
	hud_append_num(where, p.y);
	ft_strlcat(where, ", ", 64);
	hud_append_num(where, p.z);
}

/**
 * name: "Selected: Sphere (3 of 14)" or "Selected: Camera",
 * where: its position. Both buffers hold 64 characters.
 */
void	hud_selection(t_program *prog, char *name, char *where)
{
	t_object	*cur;
	int			index;
	int			count;

	ft_strlcpy(name, "Selected: Camera", 64);
	describe_position(prog->scene->camera.center, where);
	if (prog->is_cam || !prog->selected)
		return ;
	index = 0;
	count = 0;
	cur = prog->scene->objects;
	while (cur)
	{
		count++;
		if (cur == prog->selected)
			index = count;
		cur = cur->next;
	}
	ft_strlcpy(name, "Selected: ", 64);
	ft_strlcat(name, prog->selected->name, 48);
	ft_strlcat(name, " (", 64);
	hud_append_int(name, index);
	ft_strlcat(name, " of ", 64);
	hud_append_int(name, count);
	ft_strlcat(name, ")", 64);
	describe_position(object_center(prog->selected), where);
}
