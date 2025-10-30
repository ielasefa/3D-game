/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 02:13:37 by iel-asef          #+#    #+#             */
/*   Updated: 2025/10/30 02:13:40 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static int is_valid_char(char c)
{
    return (c == '0' || c == '1' || c == 'N' || c == 'S' || 
            c == 'E' || c == 'W' || c == ' ');
}

static int is_surrounded_by_walls(char **map, int i, int j)
{
    if (i > 0 && j > 0 && map[i][j] != '1')
    {
        if (map[i-1][j] == ' ' || map[i+1][j] == ' ' || 
            map[i][j-1] == ' ' || map[i][j+1] == ' ')
            return (0);
    }
    return (1);
}

int validate_map(char **map)
{
    int i = 0;
    int j;
    int player = 0;
    
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (!is_valid_char(map[i][j]))
                return (0);
            if (map[i][j] == 'N' || map[i][j] == 'S' || 
                map[i][j] == 'E' || map[i][j] == 'W')
            {
                player++;
                if (!is_surrounded_by_walls(map, i, j))
                    return (0);
            }
            j++;
        }
        i++;
    }
    return (player == 1);
}