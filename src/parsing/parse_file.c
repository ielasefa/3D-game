/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:50:13 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/17 14:46:29 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"
#include <stddef.h>

int	has_cub_extension(char *path)
{
    char	*extension;

    extension = ft_strrchr(path, '.');
    if (extension == NULL)
        return (ft_putendl_fd(INVALID_PATH, 2), 1);
    if (ft_strcmp(extension, ".cub") == 0)
        return (0);
    else
        return (ft_putendl_fd(INVALID_EXT, 2), 1);
}

int	is_empty_line(char *line)
{
    int	i;

    i = 0;
    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    return (line[i] == '\n' || line[i] == '\0');
}

static char *trim_line_inplace(char *line)
{
    char *start;
    char *end;
    size_t len;

    if (!line) return NULL;

    len = ft_strlen(line);
    if (len && line[len - 1] == '\n')
        line[len - 1] = '\0';

    end = line + ft_strlen(line);
    while (end > line && (end[-1] == ' ' || end[-1] == '\t'))
        --end;
    *end = '\0';

    start = line;
    while (*start == ' ' || *start == '\t')
        ++start;

    return start;
}

int	parse_file(char *filename, t_config *config)
{
    int		fd;
    char	*line;
    char	*view;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (print_error(ERR_INVALID_PATH), 1);

    config->map = NULL;
    config->map_h = 0;
    config->map_w = 0;

    while ((line = get_next_line(fd)))
    {
        if (is_empty_line(line))
        {
            free(line);
            continue;
        }
        view = trim_line_inplace(line);
        if (view && *view)
            parse_identifier(config, view);
        free(line);

    }
    close(fd);
    return (0);

}
