/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:49:59 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/11 14:49:28 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include <mlx.h>
# include "gnl/get_next_line.h"


/* ERRORS */
# define ERR_INVALID_PATH    1
# define ERR_INVALID_EXT     2
# define ERR_INVALID_RGB     3
# define ERR_INVALID_MAP     4
# define ERR_MISSING_TEXTURE 5
# define ERR_MISSING_COLOR   6
# define ERR_NO_PLAYER       7
# define ERR_MULTIPLAYER     8
# define ERR_UNKNOWN         99

typedef struct s_config
{
	char	*no_tex;
	char	*so_tex;
	char	*we_tex;
	char	*ea_tex;
	int		floor[3];
	int		ceil[3];
	char	**map;
	int		map_h;
	int		map_w;
	int		player_x;
	int		player_y;
	char	player_dir;
}	t_config;

//libft functions


// parsing functions

// resting functions

#endif