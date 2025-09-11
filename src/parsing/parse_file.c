/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:50:13 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/10 20:50:41 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

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

inline int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int parsinge(char *filename, t_maps *map_data)
{
    int fd;
    char *line;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (ft_putendl_fd("Error: can't open file", 2), 1);

    while ((line = get_next_line(fd)))
    {
        if (is_empty_line(line))
        {
            free(line);
            continue;
        }
        if (ft_strncmp(line, "NO ", 3) == 0)
            map_data->no = ft_strdup(line + 3);
        // Zid parsing dyal SO, WE, EA, F, C, map, player, etc...
        free(line);
    }
    close(fd);
    return (0);
}