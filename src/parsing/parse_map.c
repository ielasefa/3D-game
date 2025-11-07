/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:50:45 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/05 17:18:56 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static int is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == 'N' || c == 'S' ||
            c == 'E' || c == 'W' || c == ' ' || c == 'D');
}

static int get_height(char **map)
{
    int h = 0;
    while (map && map[h])
        h++;
    return h;
}

static int get_max_width(char **map)
{
    int i = 0, maxw = 0;
    while (map && map[i])
    {
        int len = (int)ft_strlen(map[i]);
        if (len > maxw)
            maxw = len;
        i++;
    }
    return maxw;
}

static char char_at(t_config *cfg, int i, int j)
{
    if (i < 0 || i >= cfg->map_h)
        return ' ';
    int len = (int)ft_strlen(cfg->map[i]);
    if (j < 0 || j >= len)
        return ' ';
    return cfg->map[i][j];
}

static void find_player(t_config *cfg)
{
    int i;
    int j;
    int count;

    i = 0;
    count = 0;
    while (i < cfg->map_h)
    {
        int len = (int)ft_strlen(cfg->map[i]);
        j = 0;
        while (j < len)
        {
            char c = cfg->map[i][j];
            if (is_player_char(c))
            {
                cfg->player_x = j;
                cfg->player_y = i;
                cfg->player_dir = c;
                count++;
            }
            j++;
        }
        i++;
    }
    if (count == 0)
        print_error(ERR_NO_PLAYER);
    if (count > 1)
        print_error(ERR_MULTIPLAYER);
}

static void ensure_closed(t_config *cfg)
{
    int i;
    int j;

    i = 0;
    while (i < cfg->map_h)
    {
        int len = (int)ft_strlen(cfg->map[i]);
        j = 0;
        while (j < len)
        {
            char c = cfg->map[i][j];
            if (c == '0' || c == 'D' || is_player_char(c))
            {
                if (char_at(cfg, i - 1, j) == ' ' ||
                    char_at(cfg, i + 1, j) == ' ' ||
                    char_at(cfg, i, j - 1) == ' ' ||
                    char_at(cfg, i, j + 1) == ' ')
                    print_error(ERR_INVALID_MAP);
            }
            j++;
        }
        i++;
    }
}

void validate_map(t_config *config)
{
    int i;
    int j;

    if (!config->map || !config->map[0])
        print_error(ERR_INVALID_MAP);

    i = 0;
    while (config->map[i])
    {
        j = 0;
        while (config->map[i][j])
        {
            if (!is_valid_map_char(config->map[i][j]))
                print_error(ERR_INVALID_MAP);
            j++;
        }
        i++;
    }

    config->map_h = get_height(config->map);
    config->map_w = get_max_width(config->map);

    find_player(config);
    ensure_closed(config);
}
