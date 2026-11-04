#include "../include/mesh.h"

/**
 * Area weighted (fn) and unit (un) normal of every face.
 */
static void	face_normals(t_smooth *sm)
{
	t_face	*faces;
	t_vec3	*pos;
	t_vec3	n;
	size_t	i;

	faces = sm->f->faces.data;
	pos = sm->f->pos.data;
	i = 0;
	while (i < sm->f->faces.len)
	{
		n = vec_cross(vec_sub(pos[faces[i].v[1]], pos[faces[i].v[0]]),
				vec_sub(pos[faces[i].v[2]], pos[faces[i].v[0]]));
		sm->fn[i] = n;
		sm->un[i] = vec_normalize(n);
		i++;
	}
}

/**
 * off[v + 1] = number of smooth faces using vertex v, then prefix sums:
 * the faces around v end up in adj[off[v]] .. adj[off[v + 1] - 1].
 */
static void	count_corners(t_smooth *sm)
{
	t_face	*faces;
	size_t	i;
	int		k;

	faces = sm->f->faces.data;
	i = 0;
	while (i < sm->f->faces.len)
	{
		k = 0;
		while (k < 3 && faces[i].smooth != 0)
		{
			sm->off[faces[i].v[k] + 1] += 1;
			k++;
		}
		i++;
	}
	i = 1;
	while (i <= sm->f->pos.len)
	{
		sm->off[i] += sm->off[i - 1];
		i++;
	}
}

static void	fill_adjacency(t_smooth *sm)
{
	t_face	*faces;
	size_t	i;
	int		k;
	int		v;

	faces = sm->f->faces.data;
	i = 0;
	while (i < sm->f->pos.len)
	{
		sm->cache[i] = sm->off[i];
		i++;
	}
	i = 0;
	while (i < sm->f->faces.len)
	{
		k = 0;
		while (k < 3 && faces[i].smooth != 0)
		{
			v = faces[i].v[k];
			sm->adj[sm->cache[v]] = (int)i;
			sm->cache[v] += 1;
			k++;
		}
		i++;
	}
}

void	smooth_free(t_smooth *sm)
{
	free(sm->fn);
	free(sm->un);
	free(sm->off);
	free(sm->adj);
	free(sm->cache);
}

/**
 * Face normals and the vertex -> faces table. cache (one slot per vertex,
 * used to share identical normals) starts empty.
 */
void	smooth_init(t_smooth *sm, t_objfile *f)
{
	size_t	i;

	sm->f = f;
	sm->fn = malloc(sizeof(t_vec3) * f->faces.len);
	sm->un = malloc(sizeof(t_vec3) * f->faces.len);
	sm->off = ft_calloc(f->pos.len + 1, sizeof(int));
	sm->adj = malloc(sizeof(int) * f->faces.len * 3);
	sm->cache = malloc(sizeof(int) * (f->pos.len + 1));
	if (!sm->fn || !sm->un || !sm->off || !sm->adj || !sm->cache)
	{
		smooth_free(sm);
		obj_fatal(f, "out of memory");
	}
	face_normals(sm);
	count_corners(sm);
	fill_adjacency(sm);
	i = 0;
	while (i <= f->pos.len)
	{
		sm->cache[i] = -1;
		i++;
	}
}
