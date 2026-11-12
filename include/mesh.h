#ifndef MESH_H
# define MESH_H

# include "minirt.h"

/*
** Triangle meshes loaded from Wavefront .obj files.
**
** A mesh is stored in its own "object space": centered on the origin and
** scaled so its largest side is 1. The scene places it with center, size and
** a rotation, so moving / scaling / rotating it never touches the triangles or
** the BVH: rays are transformed into object space instead.
*/

# define BVH_LEAF_SIZE 4
# define BVH_BINS 12
# define BVH_MAX_DEPTH 60
# define BVH_STACK_SIZE 64
/* Faces meeting at more than 60 degrees keep a hard edge (cos 60 = 0.5) */
# define CREASE_COS 0.5
# define MESH_ROT_STEP 15.0
# define MESH_SCALE_STEP 1.1

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

/* Triangle as used by the intersection test: v0 and two edges. */
typedef struct s_tri
{
	t_vec3	v0;
	t_vec3	e1;
	t_vec3	e2;
}	t_tri;

/* Shading data of a triangle: normal / uv index per corner (-1 = none). */
typedef struct s_tri_attr
{
	int	n[3];
	int	uv[3];
	int	mat;
}	t_tri_attr;

/* Inner node: first = index of the left child (right is first + 1).
** Leaf: triangles [first, first + count). */
typedef struct s_bvh_node
{
	t_aabb	box;
	int		first;
	int		count;
}	t_bvh_node;

/* Subset of a .mtl material. kd and ke are 0..255 like every other color
** (ke, the glow, may go above). ks is the reflected share (used with
** illum 3 to 7, see mtl_shine.c). */
typedef struct s_material
{
	char		*name;
	t_color		kd;
	double		dissolve;
	double		ior;
	t_texture	*map_kd;
	double		ks;
	int			illum;
	t_color		ke;
}	t_material;

typedef struct s_mesh
{
	t_tri		*tris;
	t_tri_attr	*attrs;
	int			tri_count;
	t_vec3		*normals;
	int			normal_count;
	t_vec3		*uvs;
	int			uv_count;
	t_material	*mats;
	int			mat_count;
	t_bvh_node	*nodes;
	int			node_count;
	t_vec3		center;
	double		size;
	t_vec3		rot[3];
}	t_mesh;

/* Ray in object space while walking the BVH. */
typedef struct s_mray
{
	t_vec3	o;
	t_vec3	d;
	t_vec3	inv;
	double	t_max;
	int		tri;
	double	b1;
	double	b2;
	bool	any;
}	t_mray;

/* --- .obj loading --- */

typedef struct s_arr
{
	void	*data;
	size_t	len;
	size_t	cap;
	size_t	size;
}	t_arr;

/* One triangle while loading: 0-based indices, -1 = none. */
typedef struct s_face
{
	int	v[3];
	int	uv[3];
	int	n[3];
	int	mat;
	int	smooth;
}	t_face;

typedef struct s_objfile
{
	t_scene		*scene;
	const char	*path;
	char		*buf;
	char		*mtl_buf;
	int			line;
	t_arr		pos;
	t_arr		uvs;
	t_arr		normals;
	t_arr		faces;
	t_arr		mats;
	int			cur_mat;
	int			cur_smooth;
	int			unknown;
	bool		warned_mtl;
}	t_objfile;

/* Computing normals for faces that have none (see mesh_normals.c). */
typedef struct s_smooth
{
	t_objfile	*f;
	t_vec3		*fn;
	t_vec3		*un;
	int			*off;
	int			*adj;
	int			*cache;
}	t_smooth;

/* BVH construction (binned surface area heuristic). */
typedef struct s_bvh_build
{
	t_objfile	*f;
	t_mesh		*m;
	t_aabb		*tb;
	t_vec3		*tc;
	int			*idx;
}	t_bvh_build;

typedef struct s_bin
{
	t_aabb	box;
	int		count;
}	t_bin;

typedef struct s_sweep
{
	double	area[BVH_BINS];
	int		count[BVH_BINS];
}	t_sweep;

typedef struct s_bvh_split
{
	int		axis;
	int		plane;
	double	cost;
	double	cmin;
	double	scale;
}	t_bvh_split;

/* BVH traversal stack */
typedef struct s_walk
{
	int		node[BVH_STACK_SIZE];
	double	t[BVH_STACK_SIZE];
	int		sp;
}	t_walk;

/* Loader */
void		load_obj(t_scene *scene, t_mesh *mesh, const char *path);
NR void		obj_fatal(t_objfile *f, const char *msg);
void		obj_free(t_objfile *f);
void		obj_statement(t_objfile *f, char *line);
void		obj_face(t_objfile *f, char *s);
void		obj_mtllib(t_objfile *f, char *s);
void		obj_usemtl(t_objfile *f, char *s);
void		obj_smooth_normals(t_objfile *f);
void		obj_build_mesh(t_objfile *f, t_mesh *m);
void		bvh_build(t_objfile *f, t_mesh *m);
void		obj_emit_triangle(t_objfile *f, const int *a, const int *b,
				const int *c);
void		smooth_init(t_smooth *sm, t_objfile *f);
void		smooth_free(t_smooth *sm);

/* Loader helpers */
char		*next_line(char **cursor);
char		*path_in_dir(const char *file, const char *name);
void		arr_push(t_objfile *f, t_arr *a, const void *elem);
char		*skip_ws(char *s);
bool		is_blank(char c);
bool		is_keyword(char *s, const char *kw, char **rest);
bool		is_ignored_statement(char *s);
bool		parse_real(char **s, double *out);
bool		parse_index(char **s, long *out);
char		*next_token(char **s);
void		mtl_load(t_objfile *f, const char *path);
void		mtl_statement(t_objfile *f, const char *path, int *cur,
				char *line);
void		obj_warn(const char *path, const char *msg, const char *what);
void		mtl_shine(t_material *m, char *line);
void		mtl_apply_shine(const t_material *mat, const t_object *obj,
				t_hit_record *rec);

/* BVH helpers */
bool		find_split(t_bvh_build *b, const t_bvh_node *n, t_bvh_split *best);
int			bin_of(const t_bvh_split *s, t_vec3 c);
int			bvh_partition(t_bvh_build *b, const t_bvh_node *n,
				const t_bvh_split *s);
t_aabb		centroid_bounds(t_bvh_build *b, const t_bvh_node *n);
void		node_init(t_bvh_build *b, int node, int first, int count);
NR void		bvh_fail(t_bvh_build *b, const char *msg);
t_aabb		aabb_empty(void);
void		aabb_grow(t_aabb *b, t_vec3 p);
void		aabb_merge(t_aabb *b, const t_aabb *o);
double		aabb_area(const t_aabb *b);
double		vec_axis(t_vec3 v, int axis);

/* Intersection and shading */
bool		tri_hit(const t_tri *t, t_mray *r, int index);
bool		bvh_walk(const t_mesh *m, t_mray *r);
bool		box_hit(const t_aabb *b, const t_mray *r, double *t_near);
void		mesh_fill_hit(const t_mesh *m, const t_ray *ray, const t_mray *r,
				t_hit_record *rec);

/* Transform */
t_vec3		mat_mul(const t_vec3 m[3], t_vec3 v);
t_vec3		mat_tmul(const t_vec3 m[3], t_vec3 v);
void		mesh_rotate(t_mesh *m, int axis, double degrees);
void		mesh_set_rotation(t_mesh *m, t_vec3 degrees);
void		free_materials(t_material *mats, int count);

#endif
