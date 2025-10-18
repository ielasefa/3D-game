/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:50:05 by iel-asef          #+#    #+#             */
/*   Updated: 2025/10/18 02:16:29 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"
#include <string.h>

static void	init_config(t_config *config)
{
    config->no_tex = NULL;
    config->so_tex = NULL;
    config->we_tex = NULL;
    config->ea_tex = NULL;
    config->floor[0] = -1;
    config->floor[1] = -1;
    config->floor[2] = -1;
    config->ceil[0] = -1;
    config->ceil[1] = -1;
    config->ceil[2] = -1;
    config->map = NULL;
    config->map_h = 0;
    config->map_w = 0;
    config->player_x = -1;
    config->player_y = -1;
    config->player_dir = '\0';
}

int	main(int ac, char **av)
{
    t_config	config;
    t_game      game;
    
    // Clear game to avoid uninitialized fields (mouse state, textures, etc.)
    memset(&game, 0, sizeof(game));
  
    if (ac != 2)
        return (ft_putendl_fd(USAGE_ERR, 2), 1);
    if (has_cub_extension(av[1]))
        return (1);
    init_config(&config);
    if (parse_file(av[1], &config))
        return (1);
    validate_map(&config);
    creat_window(&game,&config);
    return (0);
}