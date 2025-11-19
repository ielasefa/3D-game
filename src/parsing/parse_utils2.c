/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 01:30:21 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/10 19:29:36 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

int is_map_line(char *line)
{
    int i = 0;

    while (line[i])
    {
        if (!is_valid_map_char(line[i]))
            return (0);
        i++;
    }
    return (1);
}


int is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == 'N' || c == 'S' ||
            c == 'E' || c == 'W' || c == ' ' || c == 'D');
}

int get_height(char **map)
{
    int h = 0;
    while (map && map[h])
        h++;
    return h;
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int is_player_char(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}