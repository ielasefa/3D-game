/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:02:16 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/09/23 09:17:07 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int is_valid_position(t_config *config, float x, float y)
{
    int map_x = (int)(x / TILE);
    int map_y = (int)(y / TILE);
    
    // float padding = 5.0f;
    // int map_x_left = (int)((x - padding) / TILE);
    // int map_x_right = (int)((x + padding) / TILE);
    // int map_y_top = (int)((y - padding) / TILE);
    // int map_y_bottom = (int)((y + padding) / TILE);
    
    return (map_at(config, map_x, map_y) != '1');
            // map_at(config, map_x_left, map_y) != '1' &&
            // map_at(config, map_x_right, map_y) != '1' &&
            // map_at(config, map_x, map_y_top) != '1' &&
            // map_at(config, map_x, map_y_bottom) != '1');
}

void rotate_player(t_game *game, int direction)
{
    float rotation_speed = 0.1f;
    
    if (direction == 1) // Right
        game->player.angle += rotation_speed;
    else if (direction == -1) // Left
        game->player.angle -= rotation_speed;
    
    if (game->player.angle < 0)
        game->player.angle += 2 * M_PI;
    if (game->player.angle >= 2 * M_PI)
        game->player.angle -= 2 * M_PI;
}

void move_player(t_game *game, t_config *config, int direction)
{
    float move_speed = 3.0f;
    float new_x = game->player.x;
    float new_y = game->player.y;

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
        new_x += cosf(game->player.angle - M_PI / 2) * move_speed;
        new_y += sinf(game->player.angle - M_PI / 2) * move_speed;
    }
    else if (direction == 'd')
    {
        new_x += cosf(game->player.angle + M_PI / 2) * move_speed;
        new_y += sinf(game->player.angle + M_PI / 2) * move_speed;
    }

    if (is_valid_position(config, new_x, game->player.y))
        game->player.x = new_x;
    if (is_valid_position(config, game->player.x, new_y))
        game->player.y = new_y;
}

int key_press(int keycode, t_game *game)
{
    t_config *config = game->config;

    if (keycode == 13 || keycode == 119) // W or w
        move_player(game, config, 'w');
    else if (keycode == 0 || keycode == 97) // A or a
        move_player(game, config, 'a');
    else if (keycode == 1 || keycode == 115) // S or s
        move_player(game, config, 's');
    else if (keycode == 2 || keycode == 100) // D or d
        move_player(game, config, 'd');
    else if (keycode == 123) // Left 
        rotate_player(game, -1);
    else if (keycode == 124) // Right 
        rotate_player(game, 1);
    else if (keycode == 53) // ESC
    {
        mlx_destroy_window(game->mlx, game->window);
        exit(0);
    }

    update_display(game, config);
    return (0);
}

