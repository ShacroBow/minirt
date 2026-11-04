#include "../include/mesh.h"

/**
 * m * v, with m given as its three rows (object -> world rotation).
 */
t_vec3	mat_mul(const t_vec3 m[3], t_vec3 v)
{
	return ((t_vec3){vec_dot(m[0], v), vec_dot(m[1], v), vec_dot(m[2], v)});
}

/**
 * transpose(m) * v: the inverse rotation (world -> object).
 */
t_vec3	mat_tmul(const t_vec3 m[3], t_vec3 v)
{
	return (vec_add(vec_add(vec_mult(m[0], v.x), vec_mult(m[1], v.y)),
			vec_mult(m[2], v.z)));
}

static void	axis_matrix(int axis, double rad, t_vec3 a[3])
{
	double	c;
	double	s;

	c = cos(rad);
	s = sin(rad);
	a[0] = (t_vec3){1.0, 0.0, 0.0};
	a[1] = (t_vec3){0.0, 1.0, 0.0};
	a[2] = (t_vec3){0.0, 0.0, 1.0};
	if (axis == 0)
	{
		a[1] = (t_vec3){0.0, c, -s};
		a[2] = (t_vec3){0.0, s, c};
	}
	else if (axis == 1)
	{
		a[0] = (t_vec3){c, 0.0, s};
		a[2] = (t_vec3){-s, 0.0, c};
	}
	else
	{
		a[0] = (t_vec3){c, -s, 0.0};
		a[1] = (t_vec3){s, c, 0.0};
	}
}

/**
 * Rotates the mesh around a world axis (0 = x, 1 = y, 2 = z).
 */
void	mesh_rotate(t_mesh *m, int axis, double degrees)
{
	t_vec3	a[3];
	t_vec3	r[3];
	int		i;

	axis_matrix(axis, degrees * M_PI / 180.0, a);
	i = 0;
	while (i < 3)
	{
		r[i] = mat_tmul(m->rot, a[i]);
		i++;
	}
	m->rot[0] = r[0];
	m->rot[1] = r[1];
	m->rot[2] = r[2];
}

/**
 * rot=x,y,z from the scene: degrees around x, then y, then z.
 */
void	mesh_set_rotation(t_mesh *m, t_vec3 degrees)
{
	m->rot[0] = (t_vec3){1.0, 0.0, 0.0};
	m->rot[1] = (t_vec3){0.0, 1.0, 0.0};
	m->rot[2] = (t_vec3){0.0, 0.0, 1.0};
	mesh_rotate(m, 0, degrees.x);
	mesh_rotate(m, 1, degrees.y);
	mesh_rotate(m, 2, degrees.z);
}
