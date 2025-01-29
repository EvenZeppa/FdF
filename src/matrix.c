/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 15:23:57 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 15:23:57 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"
#include <math.h>

t_mat4	mat4_identity(void)
{
	t_mat4	m;
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			m.m[i][j] = 0.0f;
			j++;
		}
		i++;
	}
	i = 0;
	while (i < 4)
	{
		m.m[i][i] = 1.0f;
		i++;
	}
	return (m);
}

t_mat4	mat4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up)
{
	t_vec3	z;
	t_vec3	x;
	t_vec3	y;
	t_mat4	view;

	z = vec3_normalize(vec3_sub(eye, target));
	x = vec3_normalize(vec3_cross(up, z));
	y = vec3_cross(z, x);
	view = mat4_identity();
	view.m[0][0] = x.x;
	view.m[0][1] = x.y;
	view.m[0][2] = x.z;
	view.m[0][3] = -vec3_dot(x, eye);
	view.m[1][0] = y.x;
	view.m[1][1] = y.y;
	view.m[1][2] = y.z;
	view.m[1][3] = -vec3_dot(y, eye);
	view.m[2][0] = z.x;
	view.m[2][1] = z.y;
	view.m[2][2] = z.z;
	view.m[2][3] = -vec3_dot(z, eye);
	return (view);
}

t_mat4	mat4_perspective(float fov, float aspect, float near, float far)
{
	t_mat4	proj;
	float	tan_half_fov;
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			proj.m[i][j] = 0.0f;
			j++;
		}
		i++;
	}
	tan_half_fov = tanf(fov * 0.5f * M_PI / 180.0f);
	proj.m[0][0] = 1.0f / (aspect * tan_half_fov);
	proj.m[1][1] = 1.0f / tan_half_fov;
	proj.m[2][2] = -(far + near) / (far - near);
	proj.m[2][3] = -(2.0f * far * near) / (far - near);
	proj.m[3][2] = -1.0f;
	return (proj);
}

t_mat4	mat4_multiply(t_mat4 a, t_mat4 b)
{
	t_mat4	result;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result.m[i][j] = 0;
			k = 0;
			while (k < 4)
			{
				result.m[i][j] += a.m[i][k] * b.m[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (result);
}

t_vec4	mat4_transform_point(t_mat4 matrix, t_vec4 point)
{
	t_vec4	t_point;

	t_point.x = matrix.m[0][0] * point.x + matrix.m[0][1]
		* point.y + matrix.m[0][2] * point.z + matrix.m[0][3] * point.w;
	t_point.y = matrix.m[1][0] * point.x + matrix.m[1][1]
		* point.y + matrix.m[1][2] * point.z + matrix.m[1][3] * point.w;
	t_point.z = matrix.m[2][0] * point.x + matrix.m[2][1]
		* point.y + matrix.m[2][2] * point.z + matrix.m[2][3] * point.w;
	t_point.w = matrix.m[3][0] * point.x + matrix.m[3][1]
		* point.y + matrix.m[3][2] * point.z + matrix.m[3][3] * point.w;
	if (fabsf(t_point.w) > 1e-6)
	{
		t_point.x /= t_point.w;
		t_point.y /= t_point.w;
		t_point.z /= t_point.w;
	}
	return (t_point);
}
