/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 17:05:17 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 17:05:17 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

# include "structs.h"

typedef struct s_draw_data
{
	t_vec4	t_p1;
	t_vec4	t_p2;
	t_vec3	p_p1;
	t_vec3	p_p2;
	int		x1;
	int		y1;
	float	z1;
	int		x2;
	int		y2;
	float	z2;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	float	dz;
}	t_draw_data;

t_vec3	project_to_screen(t_vec3 point, int screen_width, int screen_height);
int		is_point_in_frustum(t_vec4 point, float near_plane, float far_plane);
int		get_color(t_app *app, float z);
void	draw_line(t_app *app, t_vec3 p1,
			t_vec3 p2, t_mat4 view_projection_matrix);

#endif