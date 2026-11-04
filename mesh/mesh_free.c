#include "../include/mesh.h"

void	free_materials(t_material *mats, int count)
{
	int	i;

	i = 0;
	while (mats && i < count)
	{
		free(mats[i].name);
		free_texture(mats[i].map_kd);
		i++;
	}
	free(mats);
}

void	free_mesh(t_mesh *m)
{
	if (!m)
		return ;
	free(m->tris);
	free(m->attrs);
	free(m->normals);
	free(m->uvs);
	free(m->nodes);
	free_materials(m->mats, m->mat_count);
	free(m);
}

/**
 * Frees what the loader still owns (everything not yet handed to the mesh).
 */
void	obj_free(t_objfile *f)
{
	free(f->buf);
	free(f->mtl_buf);
	free(f->pos.data);
	free(f->uvs.data);
	free(f->normals.data);
	free(f->faces.data);
	free_materials(f->mats.data, (int)f->mats.len);
	ft_bzero(&f->pos, sizeof(t_arr));
	f->buf = NULL;
	f->mtl_buf = NULL;
	f->uvs.data = NULL;
	f->normals.data = NULL;
	f->faces.data = NULL;
	f->mats.data = NULL;
}

/**
 * Arrow keys on a selected mesh: up / down scale it, left / right turn it
 * around the vertical axis.
 */
void	mesh_arrow_key(t_object *obj, int keycode)
{
	t_mesh	*m;

	m = (t_mesh *)obj->shape_data;
	if (keycode == KEY_UP)
		m->size *= MESH_SCALE_STEP;
	else if (keycode == KEY_DOWN)
		m->size /= MESH_SCALE_STEP;
	else if (keycode == KEY_LEFT)
		mesh_rotate(m, 1, MESH_ROT_STEP);
	else if (keycode == KEY_RIGHT)
		mesh_rotate(m, 1, -MESH_ROT_STEP);
}
