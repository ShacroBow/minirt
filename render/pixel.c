#include "../minirt.h"

static t_color apply_gamma_if_enabled(t_color color)
{
#if ENABLE_GAMMA
    return (color_gamma(color, DISPLAY_GAMMA));
#else
    return (color);
#endif
}

#if ENABLE_AA

static t_color render_pixel(t_program *prog, t_render_params *params)
{
    t_color sum;

    sum = render_aa_sample(prog, params);
    sum = apply_gamma_if_enabled(sum);
    return (sum);
}

#else

static t_color render_pixel(t_program *prog, int x, int y,
                            double inv_w, double inv_h)
{
    double u;
    double v;
    t_ray ray;
    t_color sum;

    u = ((double)x + 0.5) * inv_w;
    v = ((double)y + 0.5) * inv_h;
    ray = create_ray(&prog->scene->camera, u, v);
    sum = trace_ray(&ray, prog->scene);
    sum = apply_gamma_if_enabled(sum);
    return (sum);
}

#endif

t_color get_pixel_color(t_program *prog, int x, int y,
                        double inv_w, double inv_h)
{
#if ENABLE_AA
    t_render_params params;

    params.x = x;
    params.y = y;
    params.inv_w = inv_w;
    params.inv_h = inv_h;
    return (render_pixel(prog, &params));
#else
    return (render_pixel(prog, x, y, inv_w, inv_h));
#endif
}
