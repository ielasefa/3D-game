/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:23:56 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 03:27:25 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static char	**create_array_with_line(const char *line)
{
	char	**arr;

	arr = malloc(sizeof(char *) * 2);
	if (!arr)
		return (NULL);
	arr[0] = ft_strdup(line);
	if (!arr[0])
	{
		free(arr);
		return (NULL);
	}
	arr[1] = NULL;
	return (arr);
}

static char	**append_line(char **arr, const char *line)
{
	char	**new_arr;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (arr[count])
		count++;
	new_arr = malloc(sizeof(char *) * (count + 2));
	if (!new_arr)
		return (NULL);
	while (i < count)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = ft_strdup(line);
	if (!new_arr[i])
	{
		free(new_arr);
		return (NULL);
	}
	new_arr[i + 1] = NULL;
	free(arr);
	return (new_arr);
}

char	**add_line_to_array(char **arr, const char *line)
{
	if (!line)
		return (arr);
	if (!arr)
		return (create_array_with_line(line));
	return (append_line(arr, line));
}

char	**duplicate_map(char **map)
{
	char	**dup;
	int		i;

	i = 0;
	if (!map)
		return (NULL);
	while (map[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (map[i])
	{
		dup[i] = ft_strdup(map[i]);
		if (!dup[i])
		{
			free_partial_dup(dup, i);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

void	check_cell(t_config *cfg, int i, int j)
{
	char	c;

	c = cfg->map[i][j];
	if (c == '0' || c == 'D' || is_player_char(c))
	{
		if (char_at(cfg, i - 1, j) == ' ' || char_at(cfg, i + 1, j) == ' '
			|| char_at(cfg, i, j - 1) == ' ' || char_at(cfg, i, j + 1) == ' ')
		{
			free_config(cfg);
			print_error(ERR_INVALID_MAP);
		}
	}
}
