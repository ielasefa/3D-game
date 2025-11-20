/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:02:16 by ahabibi-          #+#    #+#             */
/*   Updated: 2025/11/20 22:57:34 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int is_valid_position(t_config *config, float x, float y)
{
    int map_x = (int)(x / TILE);
    int map_y = (int)(y / TILE);
    
    return (map_at(config, map_x, map_y) != '1' && map_at(config, map_x, map_y) != 'D');
           
}

void rotate_player(t_game *game, int direction)
{
    float rotation_speed = 0.05f;
    
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

    if (keycode == 119) 
        move_player(game, config, 'w');
    else if (keycode == 97) 
        move_player(game, config, 'a');
    else if (keycode == 115) 
        move_player(game, config, 's');
    else if ( keycode == 100) 
        move_player(game, config, 'd');
    else if (keycode == 101) 
    {
        double ang = game->player.angle;
        double px = game->player.x;
        double py = game->player.y;
        int found = 0;
        for (double d = 0.5; d <= 1.5 && !found; d += 0.15)
        {
            int tx = (int)((px + cosf(ang) * (float)(d * TILE)) / TILE);
            int ty = (int)((py + sinf(ang) * (float)(d * TILE)) / TILE);
            if (ty < 0 || tx < 0 || ty >= game->config->map_h)
                continue;
            if ((int)ft_strlen(game->config->map[ty]) <= tx)
                continue;
            if (game->config->map[ty][tx] == 'D')
            {
                game->config->map[ty][tx] = '0';
                found = 1;
            }
            else if (game->config->map[ty][tx] == '0')
            {
                if (game->config->original_map && game->config->original_map[ty]
                    && (int)ft_strlen(game->config->original_map[ty]) > tx
                    && game->config->original_map[ty][tx] == 'D')
                {
                    game->config->map[ty][tx] = 'D';
                    found = 1;
                }
            }
        }
    }
    else if (keycode == 65361) // liser
        rotate_player(game, -1);
    else if (keycode == 65363) //limen
        rotate_player(game, 1);
    else if (keycode == 65307) // ecape
        cleanup_and_exit(game);
    update_display(game, config);
    return (0);
}

