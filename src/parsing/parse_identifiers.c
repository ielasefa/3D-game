/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:12:21 by iel-asef          #+#    #+#             */
/*   Updated: 2025/10/30 00:32:51 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static int has_double_comma(const char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i + 1])
    {
        if (str[i] == ',' && str[i + 1] == ',')
            return (1);
        i++;
    }
    return (0);
}

static int is_valid_number(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t' && !ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

void	parse_rgb(int color[3], char *s)
{
    char	**split;
    int		i;
    
    while (*s && (*s == ' ' || *s == '\t'))
        s++;
        
    if (!s || *s == ',' || s[ft_strlen(s) - 1] == ',')
        print_error(ERR_INVALID_RGB);

    if (has_double_comma(s))
        print_error(ERR_INVALID_RGB);

    split = ft_split(s, ',');
    if (!split || ft_splitlen(split) != 3)
    {
        ft_free_split(split);
        print_error(ERR_INVALID_RGB);
    }

    i = -1;
    while (++i < 3)
    {
        char *trimmed = ft_strtrim(split[i], " \t");
        if (!trimmed || !*trimmed || !is_valid_number(trimmed))
        {
            ft_free_split(split);
            free(trimmed);
            print_error(ERR_INVALID_RGB);
        }
        color[i] = ft_atoi(trimmed);
        free(trimmed);
        if (color[i] < 0 || color[i] > 255)
        {
            ft_free_split(split);
            print_error(ERR_INVALID_RGB);
        }
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