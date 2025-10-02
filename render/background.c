#include "../minirt.h"

#if ENABLE_BG
/* Sky gradient using cached camera up_dir; assumes ray.direction is normalized */
t_color bg_color_from_dir(const t_scene *scene, const t_vec3 *dir)
{
    /* create_ray already normalizes direction; avoid extra normalize here */
    double proj = vec_dot(*dir, scene->camera.up_dir);
    double t = 0.5 * (proj + 1.0);

    t_color white = (t_color){255.0, 255.0, 255.0};
    t_color blue = (t_color){127.5, 178.5, 255.0}; /* (0.5,0.7,1.0)*255 */
    return color_add(color_scale(white, 1.0 - t), color_scale(blue, t));
}
#else
/* Stub when background is disabled */
t_color bg_color_from_dir(const t_scene *scene, const t_vec3 *dir)
{
    (void)scene;
    (void)dir;
    return (t_color){0, 0, 0};
}
#endif
