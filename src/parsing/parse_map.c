/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:50:45 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/16 18:09:48 by ahabibi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static void	find_player(t_config *config)
{
    int	y;
    int	x;
    int	player_count;

    player_count = 0;
    y = 0;
    while (y < config->map_h)
    {
        x = 0;
        while (config->map[y] && config->map[y][x])
        {
            if (is_player_char(config->map[y][x]))
            {
                config->player_x = x;
                config->player_y = y;
                config->player_dir = config->map[y][x];
                player_count++;
            }
            x++;
        }
        y++;
    }
    if (player_count == 0)
        print_error(ERR_NO_PLAYER);
    if (player_count > 1)
        print_error(ERR_MULTIPLAYER);
}

static int	is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' || is_player_char(c));
}

static void	validate_map_borders(t_config *config)
{
    int	y;
    int	x;

    // Check first and last row
    y = 0;
    while (y < config->map_h)
    {
        if (y == 0 || y == config->map_h - 1)
        {
            x = 0;
            while (config->map[y][x])
            {
                if (config->map[y][x] != '1' && config->map[y][x] != ' ')
                    print_error(ERR_INVALID_MAP);
                x++;
            }
        }
        y++;
    }
}

void	validate_map(t_config *config)
{
    int	y;
    int	x;

    if (!config->map || config->map_h == 0)
        print_error(ERR_INVALID_MAP);
    
    y = 0;
    while (y < config->map_h)
    {
        x = 0;
        while (config->map[y][x])
        {
            if (!is_valid_map_char(config->map[y][x]))
                print_error(ERR_INVALID_MAP);
            x++;
        }
        y++;
    }
    
    find_player(config);
    validate_map_borders(config);
}

