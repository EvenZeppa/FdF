#ifndef VECTOR_H
# define VECTOR_H

# include "structs.h"

t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_normalize(t_vec3 v);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
t_vec3	vec3_scale(t_vec3 a, float distance);
float	vec3_dot(t_vec3 a, t_vec3 b);

#endif