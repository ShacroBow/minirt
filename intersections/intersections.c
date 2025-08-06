#include "../minirt.h"

bool	hit(const t_object *world, const t_ray *ray, double t_max,
		t_hit_record *rec)
{
	const t_object	*current_obj;
	t_hit_record	temp_rec;
	bool			hit_anything;
	double			closest_so_far;

	hit_anything = false;
	closest_so_far = t_max;
	current_obj = world;
	while (current_obj)
	{
		if (current_obj->type == SPHERE && hit_sphere(current_obj->shape_data,
				ray, closest_so_far, &temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
			rec->color = current_obj->color;
		}
		else if (current_obj->type == PLANE && hit_plane(current_obj->shape_data,
				ray, closest_so_far, &temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
			rec->color = current_obj->color;
		}
		else if (current_obj->type == CYLINDER
			&& hit_cylinder(current_obj->shape_data, ray, closest_so_far,
				&temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
			rec->color = current_obj->color;
		}
		current_obj = current_obj->next;
	}
	return (hit_anything);
}