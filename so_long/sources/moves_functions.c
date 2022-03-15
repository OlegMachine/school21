/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaya <bmaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 18:42:41 by bmaya             #+#    #+#             */
/*   Updated: 2022/03/14 13:18:04 by bmaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/so_long.h"

static void	simple_move(t_general *general, t_xy next_xy, int left_move_flag)
{
	if (left_move_flag == 1)
		make_move_optional(general, general->player.pos, next_xy, \
			general->imgs.eisenhorn_l);
	else
		make_move(general, general->player.pos, next_xy);
	if (general->map.collectibles_cnt == 0)
		mlx_string_put(general->mlx, general->window.ptr, 100, \
		2, 0xff0000, "Get out of here!");
}

void	moves_functions(t_general *general, t_xy next_xy, int left_move_flag)
{
	if (general->map.field[next_xy.y][next_xy.x] != '1')
	{
		if (general->map.field[next_xy.y][next_xy.x] == 'E' && \
			general->map.collectibles_cnt == 0)
		{
			make_move(general, general->player.pos, next_xy);
			usleep(300000);
			write(1, "You earned all clots and escaped\n", 33);
			exit (0);
		}
		else if (general->map.field[next_xy.y][next_xy.x] == 'C')
		{
			draw_texture(*general, next_xy.x * 64, next_xy.y * 64, \
				general->imgs.floor);
			general->map.collectibles_cnt--;
		}
		if (general->map.field[next_xy.y][next_xy.x] != 'E')
			simple_move(general, next_xy, left_move_flag);
		left_move_flag = 0;
	}
}
