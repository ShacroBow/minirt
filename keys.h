#ifndef KEYS_H
# define KEYS_H

# ifdef __APPLE__

/* macOS (Metal MLX) */

#  define KEY_ESC		53
#  define KEY_P			35

#  define KEY_PG_UP		116
#  define KEY_PG_DOWN	121
#  define KEY_W			13
#  define KEY_A			0
#  define KEY_S			1
#  define KEY_D			2
#  define KEY_UP		126
#  define KEY_DOWN		125
#  define KEY_LEFT		123
#  define KEY_RIGHT		124

#  define KEY_PLUS		24
#  define KEY_MINUS		27
#  define KEY_SPACE		49
#  define KEY_O			31
#  define KEY_X			7

# else

/* Linux (X11 MLX) */
#  include <X11/keysym.h>

//FUNCTION
#  define KEY_ESC		XK_Escape //65307
#  define KEY_P			XK_p //70

//NAVIGATE
#  define KEY_PG_UP		XK_Page_Up //65365
#  define KEY_PG_DOWN	XK_Page_Down //65366
#  define KEY_W			XK_w //77
#  define KEY_S			XK_s //73
#  define KEY_A			XK_a //61
#  define KEY_D			XK_d //64
#  define KEY_UP		XK_Up //65362
#  define KEY_DOWN		XK_Down //65364
#  define KEY_LEFT		XK_Left //65361
#  define KEY_RIGHT		XK_Right //65363

//ADDITIONAL STUFF
#  define KEY_PLUS		XK_equal //61
#  define KEY_MINUS		XK_minus //45
#  define KEY_SPACE		XK_space //32
#  define KEY_O			XK_o
#  define KEY_X			XK_x

# endif /* __APPLE__ */

#endif /* KEYS_H */