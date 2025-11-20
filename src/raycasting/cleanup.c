/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 22:20:00 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/20 23:08:36 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void cleanup_and_exit(t_game *game)
{
    if (!game)
        exit(0);
    if (game->mlx)
    {
        if (game->img)
            mlx_destroy_image(game->mlx, game->img);

        if (game->no_texture.img)
            mlx_destroy_image(game->mlx, game->no_texture.img);
        if (game->so_texture.img)
            mlx_destroy_image(game->mlx, game->so_texture.img);
        if (game->we_texture.img)
            mlx_destroy_image(game->mlx, game->we_texture.img);
        if (game->ea_texture.img)
            mlx_destroy_image(game->mlx, game->ea_texture.img);
        if (game->door_texture.img)
            mlx_destroy_image(game->mlx, game->door_texture.img);

        for (int i = 0; i < 12; ++i)
        {
            if (game->hand_texture[i].img)
                mlx_destroy_image(game->mlx, game->hand_texture[i].img);
        }

        if (game->window)
            mlx_destroy_window(game->mlx, game->window);
    }
    if (game->config)
        free_config(game->config);
    if (game->mlx)
    {

        mlx_destroy_display(game->mlx);
        free(game->mlx);
        game->mlx = NULL;
    }

    exit(0);
}
