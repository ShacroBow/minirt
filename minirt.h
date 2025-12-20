#ifndef MINIRT_H
#define MINIRT_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <X11/keysym.h>
#include "mlx.h"
#include "libft/libft.h"
#include <stdio.h>

/* --- Constants (single source of truth) --- */
#define WIDTH 555  // 1024
#define HEIGHT 555 // 768
#define EPSILON 1e-6
#define SHININESS 32.0
#define AA_SAMPLES 32
#define DISPLAY_GAMMA 0.6
#define ENABLE_GAMMA 1
#define ENABLE_AA 1
#define ENABLE_BG 1
#define WIN_TITLE "miniRT"
#define MOVE_SPEED_BASE 0.5
#define ROT_SPEED 0.1
#define SPEED_INCREMENT 0.5
#define FILE_SIZE 4096



//FUNCTION
# define KEY_ESC		XK_Escape //65307
# define KEY_P			XK_p //70

//NAVIGATE
# define KEY_PG_UP		XK_Page_Up //65365
# define KEY_PG_DOWN	XK_Page_Down //65366
# define KEY_W			XK_w //77
# define KEY_S			XK_s //73
# define KEY_A			XK_a //61
# define KEY_D			XK_d //64
# define KEY_UP			XK_Up //65362
# define KEY_DOWN		XK_Down //65364
# define KEY_LEFT		XK_Left //65361
# define KEY_RIGHT		XK_Right //65363

//ADDITIONAL STUFF
# define KEY_PLUS		XK_equal //61
# define KEY_MINUS		XK_minus //45
# define KEY_SPACE		XK_space //32
/* --- Core Data Structures --- */

/* Vector / Point / Color */
typedef struct s_vec3
{
	double x;
	double y;
	double z;
} t_vec3;

typedef t_vec3 t_point;
typedef t_vec3 t_color;

/* Ray */
typedef struct s_ray
{
	t_point origin;
	t_vec3 direction;
} t_ray;

/* Scene Elements */
typedef struct s_ambient_light
{
	double ratio;
	t_color color;
	bool is_set;
} t_ambient_light;

typedef struct s_camera
{
	t_point center;
	t_vec3 normal;
	double fov;
	bool is_set;
	/* Pre-calculated viewport values */
	double viewport_height;
	double viewport_width;
	t_vec3 horizontal;
	t_vec3 vertical;
	t_point lower_left_corner;
	/* Cached unit up direction for fast background */
	t_vec3 up_dir;
} t_camera;

typedef struct s_light
{
	t_point center;
	double ratio;
	t_color color;
	struct s_light *next;
} t_light;

/* Geometric Objects */
typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER
} t_object_type;

typedef struct s_sphere
{
	t_point center;
	double diameter;
} t_sphere;

typedef struct s_plane
{
	t_point point;
	t_vec3 normal;
} t_plane;

typedef struct s_cylinder
{
	t_point center;
	t_vec3 normal;
	double diameter;
	double height;
} t_cylinder;

typedef struct s_object
{
	t_object_type type;
	void *shape_data;
	t_color color;
	struct s_object *next;
} t_object;

/* Hit Record */
typedef struct s_hit_record
{
	t_point point;
	t_vec3 normal;
	t_color color;
	double t;
	bool front_face;
} t_hit_record;

/* Main Scene/Program Structure */
typedef struct s_scene
{
	t_ambient_light ambient_light;
	t_camera camera;
	t_light *lights;
	t_object *objects;
} t_scene;

typedef struct s_mlx_data
{
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_mlx_data;

typedef struct s_program
{
	t_scene *scene;
	t_mlx_data mlx;
	int aa_enabled;
	double move_speed;
	t_camera default_camera;
	char *scene_file;
} t_program;

/* --- Function Prototypes --- */

/* main.c */
int main(int argc, char **argv);

/* --- Parser --- */
t_scene *parse_scene(const char *filename, t_scene **scene);
void parse_line(char *line, t_scene *scene);
void parse_ambient(t_scene *scene, char *line);
void parse_camera(t_scene *scene, char *line);
void parse_light(t_scene *scene, char *line);
void parse_sphere(t_scene *scene, char *line);
void parse_plane(t_scene *scene, char *line);
void parse_cylinder(t_scene *scene, char *line);

/* Parser Utils */
bool parse_vector(char *str, t_vec3 *vec);
bool parse_color(char *str, t_color *color);
double ft_atof(const char *str);
char **ft_split(char const *s, char c);
int count_tokens(char **tokens);
void free_tokens(char **tokens);

/* Parser Validation */
void validate_color(t_color color, t_scene *scene);
void validate_normalized_vector(t_vec3 vec, t_scene *scene);
void validate_ratio(double ratio, t_scene *scene);
void validate_fov(double fov, t_scene *scene);
void validate_scene(t_scene *scene);

/* --- Vectors --- */
t_vec3 vec_add(t_vec3 v1, t_vec3 v2);
t_vec3 vec_sub(t_vec3 v1, t_vec3 v2);
t_vec3 vec_mult(t_vec3 v, double scalar);
t_vec3 vec_div(t_vec3 v, double scalar);
double vec_dot(t_vec3 v1, t_vec3 v2);
t_vec3 vec_cross(t_vec3 v1, t_vec3 v2);
double vec_length_squared(t_vec3 v);
double vec_length(t_vec3 v);
t_vec3 vec_normalize(t_vec3 v);

/* --- Intersections --- */
bool hit(const t_object *world, const t_ray *ray, double t_max,
		 t_hit_record *rec);
bool hit_any(const t_object *world, const t_ray *ray, double t_max);
bool hit_sphere(const t_sphere *sp, const t_ray *ray, double t_max,
				t_hit_record *rec);
bool hit_plane(const t_plane *pl, const t_ray *ray, double t_max,
			   t_hit_record *rec);
bool hit_cylinder(const t_cylinder *cy, const t_ray *ray,
				  double t_max, t_hit_record *rec);

/* --- Render --- */
void setup_camera(t_camera *cam, double aspect_ratio);
int color_to_int(t_color color);
void render(t_program *prog);

/* Shading / Rays */
t_color trace_ray(const t_ray *ray, const t_scene *scene);
t_color phong_shading(const t_hit_record *rec, const t_scene *scene, const t_vec3 *ray_dir);
t_ray create_ray(const t_camera *cam, double u, double v);

t_color bg_color_from_dir(const t_scene *scene, const t_vec3 *dir);

/* Color utils */
t_color color_mult(t_color c1, t_color c2);
t_color color_add(t_color c1, t_color c2);
t_color color_scale(t_color c, double scalar);
t_color color_gamma(t_color c, double gamma);

/* --- Utils --- */
void exit_error(const char *message);
void exit_error_with_cleanup(t_program *prog, const char *message);
void cleanup(t_program *prog);
void free_scene(t_scene *scene);
bool has_extension(const char *filename, const char *ext);

#endif