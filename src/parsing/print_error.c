/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:05:32 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/11 15:47:03 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static void	put_error(char *msg)
{
    ft_putendl_fd("Error", 2);
    ft_putendl_fd(msg, 2);
    exit(1);
}

void	print_error(int code)
{
    if (code == ERR_INVALID_PATH)
        put_error("Invalid file path");
    else if (code == ERR_INVALID_EXT)
        put_error("Invalid file extension");
    else if (code == ERR_INVALID_RGB)
        put_error("Invalid RGB values");
    else if (code == ERR_INVALID_MAP)
        put_error("Invalid map");
    else if (code == ERR_MISSING_TEXTURE)
        put_error("Missing texture");
    else if (code == ERR_MISSING_COLOR)
        put_error("Missing color");
    else if (code == ERR_NO_PLAYER)
        put_error("No player found");
    else if (code == ERR_MULTIPLAYER)
        put_error("Multiple players");
    else
        put_error("Unknown error");
}
