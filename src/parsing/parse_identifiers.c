/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:12:21 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/11 15:55:57 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	parse_rgb(int color[3], char *s)
{
    char	**split;
    int		i;

    split = ft_split(s, ',');
    if (!split || ft_splitlen(split) != 3)
        print_error(ERR_INVALID_RGB);
    i = 0;
    while (i < 3)
    {
        color[i] = ft_atoi(split[i]);
        if (color[i] < 0 || color[i] > 255)
            print_error(ERR_INVALID_RGB);
        i++;
    }
    ft_free_split(split);
}

void	parse_identifier(t_config *cfg, char *line)
{
    if (ft_strncmp(line, "NO ", 3) == 0)
        cfg->no_tex = ft_strdup(line + 3);
    else if (ft_strncmp(line, "SO ", 3) == 0)
        cfg->so_tex = ft_strdup(line + 3);
    else if (ft_strncmp(line, "WE ", 3) == 0)
        cfg->we_tex = ft_strdup(line + 3);
    else if (ft_strncmp(line, "EA ", 3) == 0)
        cfg->ea_tex = ft_strdup(line + 3);
    else if (line[0] == 'F' && line[1] == ' ')
        parse_rgb(cfg->floor, line + 2);
    else if (line[0] == 'C' && line[1] == ' ')
        parse_rgb(cfg->ceil, line + 2);
    else if (is_map_line(line))
    {
        cfg->map = add_line_to_array(cfg->map, line);
        cfg->map_h++;
        if ((int)ft_strlen(line) > cfg->map_w)
            cfg->map_w = ft_strlen(line);
    }
    else if (line[0] != '\0')
        print_error(ERR_UNKNOWN);
}

int	is_map_line(char *line)
{
    int	i;

    i = 0;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '0' && line[i] != '1' &&
            !is_player_char(line[i]))
            return (0);
        i++;
    }
    return (1);
}