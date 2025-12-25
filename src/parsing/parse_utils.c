/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:23:56 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 03:08:32 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

char	**add_line_to_array(char **arr, const char *line)
{
	char	**new_arr;
	int		count;
	int		i;

	if (!line)
		return (arr);
	if (!arr)
	{
		new_arr = malloc(sizeof(char *) * 2);
		if (!new_arr)
			return (NULL);
		new_arr[0] = ft_strdup(line);
		if (!new_arr[0])
		{
			free(new_arr);
			return (NULL);
		}
		new_arr[1] = NULL;
		return (new_arr);
	}
	count = 0;
	while (arr[count])
		count++;
	new_arr = malloc(sizeof(char *) * (count + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i++] = ft_strdup(line);
	if (!new_arr[i - 1])
	{
		free(new_arr);
		return (NULL);
	}
	new_arr[i] = NULL;
	free(arr);
	return (new_arr);
}

char	**duplicate_map(char **map)
{
	char	**dup;
	int		i;

	if (!map)
		return (NULL);
	i = 0;
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
			while (i > 0)
			{
				i--;
				free(dup[i]);
			}
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

void	check_cell(t_config *cfg, int i, int j)
{
	char c;

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