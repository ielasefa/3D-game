/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:22:03 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/05 17:22:20 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"
#include <math.h>

int	mouse_press(int button, int x, int y, t_game *game)
{
    (void)button;
    (void)x;
    (void)y;
    game->mouse.is_pressed = 1;
    mlx_mouse_move(game->mlx, game->window, game->win_w / 2, game->win_h / 2);
    game->mouse.prev_x = game->win_w / 2;
    game->mouse.prev_y = game->win_h / 2;
    mlx_mouse_hide(game->mlx, game->window);
    return (0);
}

int	mouse_release(int button, int x, int y, t_game *game)
{
    (void)button;
    (void)x;
    (void)y;
    game->mouse.is_pressed = 0;
    return (0);
}

int	mouse_move(int x, int y, t_game *game)
{
    int	center_x;
    int	delta_x;

    (void)y;
    center_x = game->win_w / 2;

    delta_x = x - center_x;
    if (delta_x > -3 && delta_x < 3)
        return (0);
    if (delta_x > 80)
        delta_x = 80;
    if (delta_x < -80)
        delta_x = -80;

    if (delta_x != 0)
    {
        game->player.angle += delta_x * SENSITIVITY;
        while (game->player.angle < 0)
            game->player.angle += 2 * M_PI;
        while (game->player.angle >= 2 * M_PI)
            game->player.angle -= 2 * M_PI;
        mlx_mouse_move(game->mlx, game->window, center_x, game->win_h / 2);
        draw_mini_and_rays(game, game->config);
    }

    return (0);
}
