#ifndef CAMERA_H
# define CAMERA_H

# include "defs.h"
# include "structs.h"
# include "vector.h"
# include "matrix.h"
# include <math.h>
# include <stdio.h>

typedef struct s_range
{
	float	min_x;
	float	min_y;
	float	min_z;
	float	max_x;
	float	max_y;
	float	max_z;
}	t_range;

typedef struct s_angle
{
	float	cos;
	float	sin;
}	t_angle;

void		position_camera(t_vec3 *points, int num_points, t_camera *camera);
t_camera	create_camera(void);
void		camera_move_forward(t_camera *camera, float distance);
void		camera_strafe(t_camera *camera, float distance);
void		camera_move_up(t_camera *camera, float distance);
void		camera_rotate_yaw(t_camera *camera, float angle);
void		camera_rotate_pitch(t_camera *camera, float angle);

#endif