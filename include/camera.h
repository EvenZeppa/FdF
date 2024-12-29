#ifndef CAMERA_H
# define CAMERA_H

# include "defs.h"
# include "structs.h"
# include "vector.h"
# include "matrix.h"
# include <math.h>

t_camera	create_camera();
void		camera_move_forward(t_camera* camera, float distance);
void		camera_strafe(t_camera* camera, float distance);
void		camera_move_up(t_camera* camera, float distance);
void		camera_rotate_yaw(t_camera* camera, float angle);
void		camera_rotate_pitch(t_camera* camera, float angle);

#endif