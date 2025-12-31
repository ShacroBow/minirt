#ifndef MINIRT_H
# define MINIRT_H

# define _GNU_SOURCE // needed for M_PI macro (only for vscode c/c++ extension)
# define DEBUG 1

# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <mlx.h>
# include "../libft/libft.h"
# include "keys.h"
# include "camera.h"
# include <stdio.h>
# include <sys/time.h>

/* --- Constants (single source of truth) --- */
# define WIDTH 512  // 1024
# define HEIGHT 512 // 768
# define EPSILON 1e-6 // 0.000001
# define FILE_SIZE 4096 // scene.rt
# define TEXTURE_FILE_SIZE 4000000 // texture.ppm
# define WIN_TITLE "minirt"
# define SPEED_INCREMENT 0.2
# define MOVE_SPEED_BASE 0.4
# define SHININESS 32.0
# define GRID 3 //for render_utils
# define INV_GRID 0.33333333333333 //for render_utils

# define ENABLE_AA 0
# define AA_SAMPLES 8

# define ENABLE_GAMMA 1
# define DISPLAY_GAMMA 0.6

# define ENABLE_BG 1

# define ENABLE_REFLECTIONS 1
# define MAX_REFLECTION_DEPTH 7

# define ENABLE_PIXEL_STEP 1 //downscaling
# define PIXEL_STEP_INC 1
# define PIXEL_STEP_MAX 100
# define PIXEL_STEP_MIN 1

# define ROT_SPEED 0.2

/* --- Core Data Structures --- */

/* Vector / Point / Color */
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef t_vec3	t_point;
typedef t_vec3	t_color;

/* Ray */
typedef struct s_ray
{
	t_point	origin;
	t_vec3	direction;
}	t_ray;

/* Scene Elements */
typedef struct s_ambient_light
{
	double	ratio;
	t_color	color;
	bool	is_set;
}	t_ambient_light;

typedef struct s_camera
{
	t_point	center;
	t_vec3	normal;
	double	fov;
	bool	is_set;
	/* Pre-calculated viewport values */
	double	viewport_height;
	double	viewport_width;
	t_vec3	horizontal;
	t_vec3	vertical;
	t_point	lower_left_corner;
	/* Cached unit up direction for fast background */
	t_vec3	up_dir;
}	t_camera;

typedef struct s_light
{
	t_point			center;
	double			ratio;
	t_color			color;
	struct s_light	*next;
}	t_light;

/* Geometric Objects */
typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE
}	t_object_type;

typedef struct s_sphere
{
	t_point	center;
	double	diameter;
}	t_sphere;

typedef struct s_plane
{
	t_point	point;
	t_vec3	normal;
}	t_plane;

typedef struct s_cylinder
{
	t_point	center;
	t_vec3	normal;
	double	diameter;
	double	height;
}	t_cylinder;

typedef struct s_cone
{
	t_point	center;
	t_vec3	center_dir;
	t_vec3	apex;
	double	diameter;
	double	height;
}	t_cone;

typedef struct s_cap
{
	t_point	center;
	t_vec3	normal;
	double	r2;
	int		invert;
}	t_cap;

typedef struct s_object
{
	t_object_type		type;
	void				*shape_data;
	t_color				color;
	double				reflectivity;
	bool				has_checkerboard;
	t_color				checker_color;
	bool				has_texture;
	bool				has_bump;
	bool				bump_enabled;
	struct s_texture	*texture;
	struct s_texture	*bump;
	double				uv_scale_u;
	double				uv_scale_v;
	struct s_object		*next;
}	t_object;

/* Texture structure (PPM loader) */
typedef struct s_texture
{
	int				width;
	int				height;
	int				channels;
	unsigned char	*data;
}	t_texture;

/* Hit Record */
typedef struct s_hit_record
{
	t_point			point;
	t_vec3			normal;
	t_color			color;
	double			reflect;
	bool			has_checkerboard;
	t_color			checker_color;
	int				type;
	struct s_object	*obj;
	double			reflect_depth;
	double			t;
	bool			front_face;
}	t_hit_record;

/* Main Scene/Program Structure */
typedef struct s_scene
{
	t_ambient_light	ambient_light;
	t_camera		camera;
	t_light			*lights;
	t_object		*objects;
}	t_scene;

typedef struct s_mlx_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_mlx_data;

typedef struct s_program
{
	t_scene		*scene;
	t_mlx_data	mlx;
	int			aa_enabled;
	double		move_speed;
	int			pixel_step;
	t_camera	default_camera;
	char		*scene_file;
	long		program_start_time;
	long		render_start_time;
	long		ray_count;
	long		shading_time;
	long		intersect_time;
}	t_program;

/* --- Function Prototypes --- */

/* main.c */
int			main(int argc, char **argv);

/* --- Parser --- */
t_scene		*parse_scene(const char *filename, t_scene **scene);
void		read_file(int fd, char *content, t_scene *scene);
void		lint_scene(char *filename, t_scene *scene);
bool		is_ignorable(const char *s);

/* Linter Utils */
bool		is_valid_float(char *str);
bool		check_range(double val, double min, double max);
bool		check_color_fmt(char *str);
bool		check_vector_fmt(char *str);
bool		check_normalized(char *str);

/* Linter Elements */
void		lint_ambient(char *line, t_scene *scene);
void		lint_camera(char *line, t_scene *scene);
void		lint_light(char *line, t_scene *scene);
void		lint_sphere(char *line, t_scene *scene);
void		lint_plane(char *line, t_scene *scene);
void		lint_cylinder(char *line, t_scene *scene);
void		lint_cone(char *line, t_scene *scene);

/* --- Vectors --- */
void		parse_line(char *line, t_scene *scene);
void		parse_ambient(t_scene *scene, char *line);
void		parse_camera(t_scene *scene, char *line);
void		parse_light(t_scene *scene, char *line);
void		parse_sphere(t_scene *scene, char *line);
void		parse_plane(t_scene *scene, char *line);
void		parse_cylinder(t_scene *scene, char *line);
void		parse_cone(t_scene *scene, char *line);

/* Parser Utils */
bool		parse_vector(char *str, t_vec3 *vec);
bool		check_ppm_filename(char *str);
void		add_light(t_scene *scene, t_light *new_light);
void		add_object(t_scene *scene, t_object *new_obj);
double		ft_atof(const char *str);
char		**ft_split(char const *s, char c);

/* --- Vectors --- */
t_vec3		vec_add(t_vec3 v1, t_vec3 v2);
t_vec3		vec_sub(t_vec3 v1, t_vec3 v2);
t_vec3		vec_mult(t_vec3 v, double scalar);
t_vec3		vec_div(t_vec3 v, double scalar);
double		vec_dot(t_vec3 v1, t_vec3 v2);
t_vec3		vec_cross(t_vec3 v1, t_vec3 v2);
double		vec_lensqrt(t_vec3 v);
double		vec_len(t_vec3 v);
t_vec3		vec_normalize(t_vec3 v);
t_vec3		vec_reflect(t_vec3 v, t_vec3 n);

/* --- Intersections --- */
bool		hit(const t_object *world, const t_ray *ray, double t_max, \
				t_hit_record *rec);
bool		hit_any(const t_object *world, const t_ray *ray, double t_max);
bool		hit_sphere(const t_sphere *sp, const t_ray *ray, double t_max, \
				t_hit_record *rec);
bool		hit_plane(const t_plane *pl, const t_ray *ray, double t_max, \
				t_hit_record *rec);
bool		hit_cylinder(const t_cylinder *cy, const t_ray *ray,
				double t_max, t_hit_record *rec);
bool		hit_cone(const t_cone *cy, const t_ray *ray, double t_max, \
				t_hit_record *rec);
t_vec3		cone_coeffs(const t_cone *cy, const t_ray *ray);
bool		quadratic(double a, double b, double c, double *t);

/* --- Render --- */
int			color_to_int(t_color color);
void		render(t_program *prog);
t_color		trace_ray(const t_ray *ray, t_program *prog);
t_color		get_checker_color(const t_hit_record *rec);
/* Texture loader / sampler (PPM, clamp behaviour) */
t_texture	*load_ppm(const char *path);
void		free_texture(t_texture *tex);
t_color		sample_texture(const t_texture *tex, double u, double v);

/* Render utils */

/* Render context: cache per-frame values to avoid repeated computation */
typedef struct s_render_ctx
{
	t_program	*prog;
	double		inv_w;
	double		inv_h;
	double		inv_aa_samples;
}	t_render_ctx;

long		get_time_ms(void);
long		get_time_us(void);
double		fast_rand(unsigned long *seed);
t_color		get_aa_sample(t_render_ctx *ctx, int x, int y);
t_color		render_pixel(t_render_ctx *ctx, int x, int y);
void		init_render_utils(t_program *prog, t_render_ctx *ctx);
void		update_render_stats(t_program *prog, long *t_start, \
								bool is_shading);
t_color		handle_reflection(const t_ray *ray, t_hit_record *rec, \
				t_program *prog, int depth);
t_color		trace_ray_recursive(const t_ray *ray, t_program *prog, int depth);

/* Shading / Rays */
t_color		phong_shading(const t_hit_record *rec, const t_scene *scene, \
						const t_vec3 *ray_dir);
t_ray		create_ray(const t_camera *cam, double u, double v);

/* Shading helpers */
t_color		calculate_ambient(const t_ambient_light *ambient, \
					const t_color *material_color);
t_color		diffuse(const t_light *light, \
					const t_hit_record *rec, const t_vec3 *light_dir);
t_color		specular(const t_light *light, \
					const t_hit_record *rec, const t_vec3 *view_dir, \
					const t_vec3 *light_dir);

t_color		bg_color_from_dir(const t_scene *scene, const t_vec3 *dir);

/* Color utils */
t_color		color_mult(t_color c1, t_color c2);
t_color		color_add(t_color c1, t_color c2);
t_color		color_scale(t_color c, double scalar);
t_color		color_gamma(t_color c, double gamma);

/* --- Utils --- */
void		exit_error(const char *message);
void		exit_cleanup(t_program *prog, const char *message);
void		cleanup(t_program *prog);
void		free_scene(t_scene *scene);
void		erorr(t_scene *scene, void *ptr, const char *message);
bool		has_extension(const char *filename, const char *ext);

/* Debug helpers */
void		print_objects_status(const t_scene *scene);

/* Texture Loader */
t_texture	*load_ppm(const char *path);
void		free_texture(t_texture *tex);
t_color		sample_texture(const t_texture *tex, double u, double v);
int			ppm_header(const char *s, int *i, int *w, int *h);
double		clamp01(double x);
int			compute_uv(const t_hit_record *rec, double *u, double *v);
double		compute_uv_scale(double size_m, int tex_dim_pixels);
/* Bump mapping */
void		apply_bump(t_hit_record *rec, const t_ray *ray);

#endif