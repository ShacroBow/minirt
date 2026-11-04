#include "../include/mesh.h"

static void	obj_init(t_objfile *f, t_scene *scene, const char *path)
{
	ft_bzero(f, sizeof(*f));
	f->scene = scene;
	f->path = path;
	f->pos.size = sizeof(t_vec3);
	f->uvs.size = sizeof(t_vec3);
	f->normals.size = sizeof(t_vec3);
	f->faces.size = sizeof(t_face);
	f->mats.size = sizeof(t_material);
	f->cur_mat = -1;
	f->cur_smooth = 1;
}

/**
 * Prints "Error: <file>:<line>: <msg>", frees the loader and the scene and
 * exits, like every other scene error.
 */
void	obj_fatal(t_objfile *f, const char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd((char *)f->path, 2);
	if (f->line > 0)
	{
		ft_putstr_fd(":", 2);
		ft_putnbr_fd(f->line, 2);
	}
	ft_putstr_fd(": ", 2);
	ft_putendl_fd((char *)msg, 2);
	obj_free(f);
	erorr(f->scene, NULL, NULL);
}

static void	obj_parse_all(t_objfile *f)
{
	char	*cursor;
	char	*line;

	cursor = f->buf;
	line = next_line(&cursor);
	while (line)
	{
		f->line++;
		obj_statement(f, line);
		line = next_line(&cursor);
	}
	f->line = 0;
}

static void	print_stats(const char *path, const t_mesh *m, long ms)
{
	printf("Loaded %s: %d triangles, %d normals, %d uvs, %d materials, "
		"%d BVH nodes (%ld ms)\n", path, m->tri_count, m->normal_count,
		m->uv_count, m->mat_count, m->node_count, ms);
}

/**
 * Loads a Wavefront .obj (and its .mtl files) into mesh: triangles in object
 * space, normals, texture coordinates, materials and the BVH.
 */
void	load_obj(t_scene *scene, t_mesh *mesh, const char *path)
{
	t_objfile	f;
	size_t		len;
	long		start;

	start = get_time_ms();
	obj_init(&f, scene, path);
	f.buf = read_whole_file(path, &len);
	if (!f.buf)
		obj_fatal(&f, "cannot read file");
	if (ft_strlen(f.buf) != len)
		obj_fatal(&f, "not a text file");
	obj_parse_all(&f);
	if (f.faces.len == 0)
		obj_fatal(&f, "no faces");
	if (f.unknown > 0)
		obj_warn(path, "skipped unsupported statements (curves, surfaces)",
			NULL);
	obj_smooth_normals(&f);
	obj_build_mesh(&f, mesh);
	bvh_build(&f, mesh);
	print_stats(path, mesh, get_time_ms() - start);
	obj_free(&f);
}
