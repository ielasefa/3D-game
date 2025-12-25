/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:50:45 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 03:08:54 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

int	get_max_width(char **map)
{
	int	i;
	int	maxw;
	int	len;

	maxw = 0;
	i = 0;
	while (map && map[i])
	{
		len = (int)ft_strlen(map[i]);
		if (len > maxw)
			maxw = len;
		i++;
	}
	return (maxw);
}

char	char_at(t_config *cfg, int i, int j)
{
	int	len;

	if (i < 0 || i >= cfg->map_h)
		return (' ');
	len = (int)ft_strlen(cfg->map[i]);
	if (j < 0 || j >= len)
		return (' ');
	return (cfg->map[i][j]);
}

void	find_player(t_config *cfg)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < cfg->map_h)
	{
		scan_row_for_player(cfg, i, &count);
		i++;
	}
	if (count == 0)
		print_error(ERR_NO_PLAYER);
	if (count > 1)
		print_error(ERR_MULTIPLAYER);
}

void	ensure_closed(t_config *cfg)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (i < cfg->map_h)
	{
		len = (int)ft_strlen(cfg->map[i]);
		j = 0;
		while (j < len)
		{
			check_cell(cfg, i, j);
			j++;
		}
		i++;
	}
}

void	validate_map(t_config *config)
{
	int	i;
	int	j;

	if (!config->map || !config->map[0])
		print_error(ERR_INVALID_MAP);
	i = 0;
	while (config->map[i])
	{
		j = 0;
		while (config->map[i][j])
		{
			if (!is_valid_map_char(config->map[i][j]))
			{
				free_config(config);
				print_error(ERR_INVALID_MAP);
			}
			j++;
		}
		i++;
	}
	config->map_h = get_height(config->map);
	config->map_w = get_max_width(config->map);
	find_player(config);
	ensure_closed(config);
}
