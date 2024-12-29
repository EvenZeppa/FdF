#include "matrix.h"
#include <math.h>

t_mat4 mat4_identity() {
	t_mat4 m = {0};
	for (int i = 0; i < 4; i++) m.m[i][i] = 1.0f;
	return m;
}

t_mat4 mat4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up) {
	t_vec3 z = vec3_normalize(vec3_sub(eye, target));
	t_vec3 x = vec3_normalize(vec3_cross(up, z));
	t_vec3 y = vec3_cross(z, x);

	t_mat4 view = mat4_identity();
	view.m[0][0] = x.x; view.m[0][1] = x.y; view.m[0][2] = x.z; view.m[0][3] = -vec3_dot(x, eye);
	view.m[1][0] = y.x; view.m[1][1] = y.y; view.m[1][2] = y.z; view.m[1][3] = -vec3_dot(y, eye);
	view.m[2][0] = z.x; view.m[2][1] = z.y; view.m[2][2] = z.z; view.m[2][3] = -vec3_dot(z, eye);
	return view;
}

t_mat4 mat4_perspective(float fov, float aspect, float near, float far) {
	t_mat4 proj = {0};
	float tan_half_fov = tanf(fov * 0.5f * M_PI / 180.0f);

	proj.m[0][0] = 1.0f / (aspect * tan_half_fov);
	proj.m[1][1] = 1.0f / tan_half_fov;
	proj.m[2][2] = -(far + near) / (far - near);
	proj.m[2][3] = -(2.0f * far * near) / (far - near);
	proj.m[3][2] = -1.0f;
	return proj;
}

t_mat4 mat4_multiply(t_mat4 a, t_mat4 b) {
	t_mat4 result = {0};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}

return result;
}

t_vec4 mat4_transform_point(t_mat4 matrix, t_vec4 point) {
	float x = point.x;
	float y = point.y;
	float z = point.z;
	float w = 1.0f;

	float transformed_x = matrix.m[0][0] * x + matrix.m[0][1] * y + matrix.m[0][2] * z + matrix.m[0][3] * w;
	float transformed_y = matrix.m[1][0] * x + matrix.m[1][1] * y + matrix.m[1][2] * z + matrix.m[1][3] * w;
	float transformed_z = matrix.m[2][0] * x + matrix.m[2][1] * y + matrix.m[2][2] * z + matrix.m[2][3] * w;
	float transformed_w = matrix.m[3][0] * x + matrix.m[3][1] * y + matrix.m[3][2] * z + matrix.m[3][3] * w;

	if (fabsf(transformed_w) > 1e-6) {
		transformed_x /= transformed_w;
		transformed_y /= transformed_w;
		transformed_z /= transformed_w;
	}

	return (t_vec4){transformed_x, transformed_y, transformed_z, transformed_w};
}
