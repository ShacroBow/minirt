#include "../include/mesh.h"

/**
 * Reads a .mtl file. Problems in material files are only warnings: the
 * model still renders, with the scene color where a material is missing.
 */
void	mtl_load(t_objfile *f, const char *path)
{
	size_t	len;
	char	*cursor;
	char	*line;
	int		cur;

	f->mtl_buf = read_whole_file(path, &len);
	if (!f->mtl_buf)
	{
		obj_warn(path, "cannot read material file", NULL);
		return ;
	}
	cur = -1;
	cursor = f->mtl_buf;
	line = next_line(&cursor);
	while (line)
	{
		mtl_statement(f, path, &cur, line);
		line = next_line(&cursor);
	}
	free(f->mtl_buf);
	f->mtl_buf = NULL;
}

/**
 * "mtllib a.mtl b.mtl": material files, relative to the .obj file.
 */
void	obj_mtllib(t_objfile *f, char *s)
{
	char	*name;
	char	*path;

	name = next_token(&s);
	while (name)
	{
		path = path_in_dir(f->path, name);
		if (!path)
			obj_fatal(f, "out of memory");
		mtl_load(f, path);
		free(path);
		name = next_token(&s);
	}
}

/**
 * "usemtl name": the following faces use this material.
 */
void	obj_usemtl(t_objfile *f, char *s)
{
	char		*name;
	t_material	*mats;
	size_t		i;

	f->cur_mat = -1;
	name = next_token(&s);
	if (!name)
		return ;
	mats = f->mats.data;
	i = 0;
	while (i < f->mats.len)
	{
		if (ft_strcmp(mats[i].name, name) == 0)
		{
			f->cur_mat = (int)i;
			return ;
		}
		i++;
	}
	if (!f->warned_mtl)
		obj_warn(f->path, "unknown material, using the scene color:", name);
	f->warned_mtl = true;
}
