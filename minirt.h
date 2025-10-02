#ifndef MINIRT_H
#define MINIRT_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include "mlx.h"
#include "libft/libft.h"

/* --- Constants (single source of truth) --- */
#define WIDTH 555
#define HEIGHT 555
#define EPSILON 1e-6
#define SHININESS 32.0
#define AA_SAMPLES 16
#define DISPLAY_GAMMA 1.2
#define ENABLE_GAMMA 1
#define ENABLE_AA 1
#define ENABLE_POST_AA 42 // unused
#define POST_AA_SIZE 42 // unused
#define EDGE_AA_STRENGTH 42 // unused
#define EDGE_AA_THRESHOLD 42 // unused
#define ENABLE_BG 1

/* Normalize/guard config values */
#if AA_SAMPLES < 1
#undef AA_SAMPLES
#define AA_SAMPLES 1
#endif

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
} t_program;

/* --- Function Prototypes --- */

/* main.c */
int main(int argc, char **argv);

/* --- Parser --- */
t_scene *parse_scene(const char *filename);
void parse_line(char *line, t_scene *scene);
void parse_ambient(t_scene *scene, char **tokens);
void parse_camera(t_scene *scene, char **tokens);
void parse_light(t_scene *scene, char **tokens);
void parse_sphere(t_scene *scene, char **tokens);
void parse_plane(t_scene *scene, char **tokens);
void parse_cylinder(t_scene *scene, char **tokens);

/* Parser Utils */
bool parse_vector(char *str, t_vec3 *vec);
bool parse_color(char *str, t_color *color);
double ft_atof(const char *str);
char **ft_split(char const *s, char c);
int count_tokens(char **tokens);
void free_tokens(char **tokens);

/* Parser Validation */
void validate_color(t_color color);
void validate_normalized_vector(t_vec3 vec);
void validate_ratio(double ratio);
void validate_fov(double fov);
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
t_vec3 vec_reflect(t_vec3 v, t_vec3 n);

/* --- Intersections --- */
bool hit(const t_object *world, const t_ray *ray, double t_max,
		 t_hit_record *rec);
bool hit_sphere(const t_sphere *sp, const t_ray *ray, double t_max,
				t_hit_record *rec);
bool hit_plane(const t_plane *pl, const t_ray *ray, double t_max,
			   t_hit_record *rec);
bool hit_cylinder(const t_cylinder *cy, const t_ray *ray,
				  double t_max, t_hit_record *rec);

/* --- Render --- */
void setup_camera(t_camera *cam, double aspect_ratio);
int color_to_int(t_color color);
void my_mlx_pixel_put(t_mlx_data *data, int x, int y, int color);
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
void *safe_malloc(size_t size);
void free_scene(t_scene *scene);
bool has_extension(const char *filename, const char *ext);

#endif