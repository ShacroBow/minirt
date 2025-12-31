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
#  define KEY_0			29
#  define KEY_1			18
#  define KEY_2			19
#  define KEY_3			20
#  define KEY_4			21
#  define KEY_5			23
#  define KEY_6			22
#  define KEY_7			26
#  define KEY_8			28
#  define KEY_9			25

# else

/* Linux (X11 MLX) */
#  include <X11/keysym.h>

//FUNCTION
#  define KEY_ESC		XK_Escape //65307
#  define KEY_O			XK_o //0x006f
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
#  define KEY_X			XK_x //0x0078
// Toggle bump for selected object
#  define KEY_B			XK_b //0x0062

// SELECT CAMERA AND OBJECTS
#  define KEY_0			XK_0 //0x0030
#  define KEY_1			XK_1 //0x0031
#  define KEY_2			XK_2 //0x0032
#  define KEY_3			XK_3 //0x0033
#  define KEY_4			XK_4 //0x0034
#  define KEY_5			XK_5 //0x0035
#  define KEY_6			XK_6 //0x0036
#  define KEY_7			XK_7 //0x0037
#  define KEY_8			XK_8 //0x0038
#  define KEY_9			XK_9 //0x0039

// UV scale keys (per-object): 4/5 adjust U, 6/7 adjust V

# endif /* __APPLE__ */

#endif /* KEYS_H */