/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:02:16 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/11/07 00:17:36 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"
#include <math.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*                         VALID POSITION / COLLISION                         */
/* -------------------------------------------------------------------------- */

int	is_valid_position(t_config *config, float x, float y)
{
	int	map_x;
	int	map_y;
	char	cell;

	if (!config || !config->map)
		return (0);
	map_x = (int)(x / TILE);
	map_y = (int)(y / TILE);
	cell = map_at(config, map_x, map_y);
	/* ممنوع دخول الجدران أو الأبواب المغلقة */
	return (cell != '1' && cell != 'D');
}

/* -------------------------------------------------------------------------- */
/*                                 ROTATION                                   */
/* -------------------------------------------------------------------------- */

void	rotate_player(t_game *game, int direction)
{
	float	rotation_speed;

	if (!game)
		return;
	rotation_speed = 0.05f;
	if (direction == 1)
		game->player.angle += rotation_speed;
	else if (direction == -1)
		game->player.angle -= rotation_speed;
	if (game->player.angle < 0)
		game->player.angle += 2 * M_PI;
	else if (game->player.angle >= 2 * M_PI)
		game->player.angle -= 2 * M_PI;
}

/* -------------------------------------------------------------------------- */
/*                                MOVE PLAYER                                 */
/* -------------------------------------------------------------------------- */

void	move_player(t_game *game, t_config *config, int direction)
{
	float	move_speed;
	float	new_x;
	float	new_y;
	float	dir_x;
	float	dir_y;

	if (!game || !config)
		return;
	move_speed = 3.0f;
	new_x = game->player.x;
	new_y = game->player.y;
	if (direction == 'w')
	{
		new_x += cosf(game->player.angle) * move_speed;
		new_y += sinf(game->player.angle) * move_speed;
	}
	else if (direction == 's')
	{
		new_x -= cosf(game->player.angle) * move_speed;
		new_y -= sinf(game->player.angle) * move_speed;
	}
	else if (direction == 'a')
	{
		new_x += cosf(game->player.angle - M_PI_2) * move_speed;
		new_y += sinf(game->player.angle - M_PI_2) * move_speed;
	}
	else if (direction == 'd')
	{
		new_x += cosf(game->player.angle + M_PI_2) * move_speed;
		new_y += sinf(game->player.angle + M_PI_2) * move_speed;
	}
	else if (direction == 'e')
	{
		dir_x = cosf(game->player.angle);
		dir_y = sinf(game->player.angle);
		toggle_door(config->map, config->original_map,
			game->player.x / TILE, game->player.y / TILE, dir_x, dir_y);
		return;
	}
	else if (direction == 65361) // left arrow
		return (rotate_player(game, -1));
	else if (direction == 65363) // right arrow
		return (rotate_player(game, 1));
	if (is_valid_position(config, new_x, game->player.y))
		game->player.x = new_x;
	if (is_valid_position(config, game->player.x, new_y))
		game->player.y = new_y;
}

/* -------------------------------------------------------------------------- */
/*                                 KEY PRESS                                  */
/* -------------------------------------------------------------------------- */

int	key_press(int key, t_game *game)
{
	t_config	*config;

	if (!game || !game->config)
		return (0);
	config = game->config;
	if (key == 119) // W
		move_player(game, config, 'w');
	else if (key == 97) // A
		move_player(game, config, 'a');
	else if (key == 115) // S
		move_player(game, config, 's');
	else if (key == 100) // D
		move_player(game, config, 'd');
	else if (key == 101) // E
		move_player(game, config, 'e');
	else if (key == 65361) // Left arrow
		move_player(game, config, 65361);
	else if (key == 65363) // Right arrow
		move_player(game, config, 65363);
	else if (key == 65307) // Escape
	{
		if (game->window)
			mlx_destroy_window(game->mlx, game->window);
		exit(0);
	}
	draw_mini_and_rays(game, config);
	return (0);
}
