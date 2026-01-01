#ifndef CAMERA_H
# define CAMERA_H

# include "minirt.h"

typedef struct s_camera		t_camera;
typedef struct s_vec3		t_vec3;
typedef struct s_program	t_program;
typedef struct s_object		t_object;

void		setup_camera(t_camera *cam, double aspect_ratio);
void		move_camera(t_camera *cam, t_vec3 direction, double speed);
void		rotate_camera(t_camera *cam, double pitch, double yaw);
void		move_element_camera(t_program *prog, int keycode);
void		camera_handle_key(int keycode, t_program *prog, t_object **object, \
							int *is_cam);
void		select_element(int *is_cam, t_object **object, int keycode, \
							t_program *prog);
void		move_object(t_object *object, t_vec3 direction, double speed);
void		move_element_object(t_program *prog, int keycode, t_object *object);
void		move_element(int is_cam, t_object *object, t_program *prog, \
						int keycode);
void		camera_keystuffing(int keycode, t_program *prog, \
								t_object **object, int *is_cam);
void		handle_camera_rotation(t_camera *cam, int keycode);
void		handle_speed_change(t_program *prog, int keycode);
void		handle_bump_action(t_object **object, int *is_cam);
void		handle_scaling_action(int keycode, t_object **object, int *is_cam);
#endif
