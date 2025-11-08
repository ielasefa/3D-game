/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:50:05 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/08 01:25:16 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"
#include <string.h>
#include <stdio.h>

static void	init_config(t_config *config)
{
    config->no_tex = NULL;
    config->so_tex = NULL;
    config->we_tex = NULL;
    config->ea_tex = NULL;
    config->door_tex = NULL;          /* added: initialize door_tex */
    config->floor[0] = -1;
    config->floor[1] = -1;
    config->floor[2] = -1;
    config->ceil[0] = -1;
    config->ceil[1] = -1;
    config->ceil[2] = -1;
    config->map = NULL;
    config->original_map = NULL;      /* added: initialize original_map */
    config->map_h = 0;  
    config->map_w = 0;
    config->player_x = -1;
    config->player_y = -1;
    config->player_dir = '\0';
}
int	main(int argc, char **argv)
{
	t_game		game;
	t_config	config;

	if (argc != 2)
		return (ft_putendl_fd("Usage: ./cub3d <map.cub>", 2), 1);

	memset(&config, 0, sizeof(t_config));
	memset(&game, 0, sizeof(t_game));
	init_config(&config);

	if (parse_file(argv[1], &config) != 0)
		return (ft_putendl_fd("Error: failed to parse map", 2), 1);

	validate_map(&config);

	game.config = &config;
	game.config->original_map = duplicate_map(config.map);
	if (!game.config->original_map)
		return (ft_putendl_fd("Error: duplicate_map failed", 2), 1);

	doors_init(&game.doors);
	creat_window(&game, &config);

	doors_free(&game.doors);
	ft_free_split(game.config->original_map);
	free_config(&config);
	return (0);
}
