/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:12:21 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/17 18:37:10 by iel-asef         ###   ########.fr       */
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

static int is_space(int c) 
{
     return (c == ' ' || c == '\t'); 
}

static char *trim_ws(const char *s)
{
    return ft_strtrim(s, " \t\r\n");
}

static int ends_with_xpm(const char *p)
{
    const char *dot = ft_strrchr(p, '.');
    return (dot && ft_strcmp(dot, ".xpm") == 0);
}

static int file_readable(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0) return 0;
    close(fd);
    return 1;
}

static void set_texture_or_die(char **dst, const char *raw)
{
    char *path;

    if (*dst)
    {
        print_error(ERR_INVALID_PATH);
        exit(1);
    }
    path = ft_strtrim(raw, " \t\r\n");
    if (!path || *path == '\0')
    {
        free(path);
        print_error(ERR_INVALID_PATH);
        exit(1);
    }
    if (!ends_with_xpm(path))
    {
        free(path);
        print_error(ERR_INVALID_EXT);
        exit(1);
    }
    if (!file_readable(path))
    {
        print_error_path(ERR_INVALID_PATH, path);
        free(path);
        exit(1);
    }
    *dst = path;
}

static const char *skip_id_and_spaces(const char *s, int idlen)
{
    s += idlen;
    while (*s && is_space((unsigned char)*s))
        s++;
    return s;
}


void    free_config(t_config *cfg)
{
    int i;

    if (!cfg)
        return;

    free(cfg->no_tex);
    free(cfg->so_tex);
    free(cfg->we_tex);
    free(cfg->ea_tex);
    free(cfg->door_tex);

    if (cfg->map)
    {
        i = 0;
        while (i < cfg->map_h)
        {
            free(cfg->map[i]);
            i++;
        }
        free(cfg->map);
    }

}
int	parse_rgb(int color[3], char *s)
{
    char	**split;
    int		i;
    char	*trimmed;

    if (!s)
        return (1);

    while (*s && (*s == ' ' || *s == '\t'))
        s++;

    if (*s == ',' || s[ft_strlen(s) - 1] == ',')
        return (1);

    if (has_double_comma(s))
        return (1);

    split = ft_split(s, ',');
    if (!split || ft_splitlen(split) != 3)
    {
        ft_free_split(split);
        return (1);
    }

    i = 0;
    while (i < 3)
    {
        trimmed = ft_strtrim(split[i], " \t");
        if (!trimmed || !*trimmed || !is_valid_number(trimmed))
        {
            ft_free_split(split);
            free(trimmed);
            return (1);
        }
        color[i] = ft_atoi(trimmed);
        free(trimmed);
        if (color[i] < 0 || color[i] > 255)
        {
            ft_free_split(split);
            return (1);
        }
        i++;
    }
    ft_free_split(split);
    return (0);
}

void	parse_identifier(t_config *cfg, char *line)
{
    const char *s = line;

    if (!ft_strncmp(s, "NO", 2) && is_space((unsigned char)s[2]))
        return (set_texture_or_die(&cfg->no_tex, skip_id_and_spaces(s, 2)), (void)0);
    if(!ft_strncmp(s, "SO", 2) && is_space((unsigned char)s[2]))
        return (set_texture_or_die(&cfg->so_tex, skip_id_and_spaces(s, 2)), (void)0);
    if (!ft_strncmp(s, "WE", 2) && is_space((unsigned char)s[2]))
        return (set_texture_or_die(&cfg->we_tex, skip_id_and_spaces(s, 2)), (void)0);
    if (!ft_strncmp(s, "EA", 2) && is_space((unsigned char)s[2]))
        return (set_texture_or_die(&cfg->ea_tex, skip_id_and_spaces(s, 2)), (void)0);
    if (!ft_strncmp(s, "DO", 2) && is_space((unsigned char)s[2]))
        return (set_texture_or_die(&cfg->door_tex, skip_id_and_spaces(s, 2)), (void)0);

    if (s[0] == 'F' && is_space((unsigned char)s[1]))
    {
        if (parse_rgb(cfg->floor, (char *)skip_id_and_spaces(s, 1)))
            print_error(ERR_INVALID_RGB);
            free_config(cfg);
        return;
    }
    if (s[0] == 'C' && is_space((unsigned char)s[1]))
    {
        if (parse_rgb(cfg->ceil, (char *)skip_id_and_spaces(s, 1)))
            print_error(ERR_INVALID_RGB);
           free_config(cfg);
        return;
    }

    if (is_map_line(line))
    {
        cfg->map = add_line_to_array(cfg->map, line);
        cfg->map_h++;
        if ((int)ft_strlen(line) > cfg->map_w)
            cfg->map_w = ft_strlen(line);
    }
    else if (line[0] != '\0')
        print_error(ERR_UNKNOWN);
}

