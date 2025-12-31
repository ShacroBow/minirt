/* Deprecated duplicate texture/UV implementation.
   The project now uses `texture/texture.c` for UV computation and
   sampling. Leaving this stub to avoid accidental use of two
   conflicting implementations. */

#include "../include/minirt.h"

t_color	get_checker_color(const t_hit_record *rec)
{
	(void)rec;
	return ((t_color){0,0,0});
}
