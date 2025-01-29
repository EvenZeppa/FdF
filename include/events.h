/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 17:05:40 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 17:05:40 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
# define EVENTS_H

# include "mlx.h"
# include "libft.h"
# include "defs.h"
# include "structs.h"
# include "camera.h"
# include "memory.h"

int	mouse_move(int x, int y, void *param);
int	mouse_click(int button, int x, int y, void *param);
int	key_press(int keycode, void *param);

#endif