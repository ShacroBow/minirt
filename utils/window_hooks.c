#include "minirt.h"

int	close_window(t_program *prog)
{
	cleanup(prog);
	exit(0);
	return (0);
}

int	key_hook(int keycode, t_program *prog)
{
	if (keycode == 65307)
		close_window(prog);
	return (0);
}
