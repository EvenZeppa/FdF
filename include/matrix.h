/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 17:06:14 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 17:06:14 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include "structs.h"
# include "vector.h"

t_mat4	mat4_identity(void);
t_mat4	mat4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up);
t_mat4	mat4_perspective(float fov,
			float aspect_ratio, float near_plane, float far_plane);
t_mat4	mat4_multiply(t_mat4 a, t_mat4 b);
t_vec4	mat4_transform_point(t_mat4 m, t_vec4 p);

#endif