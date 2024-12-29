#ifndef MATRIX_H
# define MATRIX_H

# include "structs.h"
# include "vector.h"

t_mat4	mat4_identity();
t_mat4	mat4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up);
t_mat4	mat4_perspective(float fov, float aspect_ratio, float near_plane, float far_plane);
t_mat4	mat4_multiply(t_mat4 a, t_mat4 b);
t_vec4	mat4_transform_point(t_mat4 m, t_vec4 p);

#endif