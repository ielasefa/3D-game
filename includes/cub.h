/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:49:59 by iel-asef          #+#    #+#             */
/*   Updated: 2025/10/30 01:31:06 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include "get_next_line.h"
# include "libft.h"
# include "minilibx-linux/mlx.h"
# include <math.h>
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

#define TILE 8
#define MINIMAP_OX 30
#define MINIMAP_OY 30
#define FOV_DEG 60
#define RAYS 90
#define MAX_SLICE_HEIGHT 320
#define COLOR_CEILING 0x87CEEB  // sky blue
#define COLOR_FLOOR   0x228B22  // forest green
#define COLOR_WALL    0xFFD700  // gold (current walls)

/* ERROR MESSAGES */
# define INVALID_PATH        "Error\nInvalid file path"
# define INVALID_EXT         "Error\nInvalid file extension"
# define USAGE_ERR           "Error\nUsage: ./cub3D <file.cub>"
// colors
#define COLOR_WHITE   0xFFFFFF  // R=255, G=255, B=255
#define COLOR_PINK    0xFF66B2  // R=255, G=102, B=178
#define COLOR_BLUE    0x0000FF  // R=0,   G=0,   B=255
#define COLOR_YELLOW  0xFFFF00  // R=255, G=255, B=0

#define SENSITIVITY 0.001f

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

typedef struct s_texture {
    void    *img;
    char    *addr;
    int     width;
    int     height;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
}   t_texture;

typedef struct s_mouse
{
    int x;
    int y;
    int prev_x;
    int prev_y;
    int is_pressed;
}   t_mouse;

typedef struct s_player {
float x;    
float y;     
float angle;  
}   t_player;

typedef struct s_game {
    void        *img;           // Image buffer pointer
    char        *img_addr;      // Image data address
    int         img_bpp;        // Bits per pixel
    int         img_line_len;   // Line length in bytes
    int         img_endian;     // Endian
    
    void        *mlx;
    void        *window;
    float       win_w;
    float       win_h;
    t_player    player;
    t_config    *config;
    t_mouse      mouse;
    t_texture   no_texture;     // North wall texture
    t_texture   so_texture;     // South wall texture
    t_texture   we_texture;     // West wall texture
    t_texture   ea_texture;     // East wall texture
}   t_game;
/// move player

void update_display(t_game *game, t_config *config);
void draw_mini_and_rays(t_game *game, t_config *config);
int close_window(t_game *game);
void update_display(t_game *game, t_config *config);
void draw_rays(t_game *game, t_config *config);
void draw_vertical_strip(t_game *game, int x, int start, int end, int color, int slice_width);
void clear_screen(t_game *game);
void init_player_from_config(t_game *game, t_config *config);
int map_at(t_config *cfg, int mx, int my);
void draw_mini_map(t_game *game, t_config *config);
void draw_pixel(t_game *game, int i, int j, int color);

int is_valid_position(t_config *config, float x, float y);
void move_player(t_game *game, t_config *config, int direction);
// int (t_config *config, float x, float y);
void rotate_player(t_game *game, int direction);

// window displays

void creat_window(t_game *game,t_config *config);
int key_press(int keycode, t_game *game);
int mouse_press(int button, int x, int y, t_game *game);
int mouse_release(int button, int x, int y, t_game *game);
int mouse_move(int x, int y, t_game *game);

//PARSING FUNCTIONS 
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