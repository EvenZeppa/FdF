#include "vector.h"
#include <math.h>

t_vec3	vec3_add(t_vec3 a, t_vec3 b) {
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b) {
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

t_vec3	vec3_normalize(t_vec3 v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return (t_vec3){v.x / length, v.y / length, v.z / length};
}

t_vec3	vec3_cross(t_vec3 a, t_vec3 b) {
	return (t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

t_vec3	vec3_scale(t_vec3 a, float distance) {
	return (t_vec3){a.x * distance, a.y * distance, a.z * distance};
}

float	vec3_dot(t_vec3 a, t_vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
