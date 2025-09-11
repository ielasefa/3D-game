/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:49:59 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/11 15:56:56 by iel-asef         ###   ########.fr       */
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
# include "get_next_line.h"
# include "libft.h"

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

/* ERROR MESSAGES */
# define INVALID_PATH        "Error\nInvalid file path"
# define INVALID_EXT         "Error\nInvalid file extension"
# define USAGE_ERR           "Error\nUsage: ./cub3D <file.cub>"

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

/* PARSING FUNCTIONS */
int		has_cub_extension(char *path);
int		is_empty_line(char *line);
int		is_player_char(char c);
int		parse_file(char *filename, t_config *config);
void	parse_rgb(int color[3], char *s);
void	parse_identifier(t_config *cfg, char *line);
int		is_map_line(char *line);
char	**add_line_to_array(char **array, char *line);
void	print_error(int code);
void	validate_map(t_config *config);

#endif