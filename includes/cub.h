/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:49:59 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/07 19:28:40 by iel-asef         ###   ########.fr       */
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

# define ERR_INVALID_PATH    1
# define ERR_INVALID_EXT     2
# define ERR_INVALID_RGB     3
# define ERR_INVALID_MAP     4
# define ERR_MISSING_TEXTURE 5
# define ERR_MISSING_COLOR   6
# define ERR_NO_PLAYER       7
# define ERR_MULTIPLAYER     8
# define ERR_UNKNOWN         99


# define MINIMAP_SCALE 8
#define TILE 8
#define MINIMAP_OX 30
#define MINIMAP_OY 30
#define FOV_DEG 60
#define RAYS 90
#define MAX_SLICE_HEIGHT 320
#define COLOR_CEILING 0x87CEEB  // sky blue
#define COLOR_FLOOR   0x228B22  // forest green
#define COLOR_WALL    0xFFD700  // gold (current walls)

# define INVALID_PATH        "Error\nInvalid file path"
# define INVALID_EXT         "Error\nInvalid file extension"
# define USAGE_ERR           "Error\nUsage: ./cub3D <file.cub>"
// colors
#define COLOR_WHITE   0xFFFFFF  // R=255, G=255, B=255
#define COLOR_PINK    0xFF66B2  // R=255, G=102, B=178
#define COLOR_BLUE    0x0000FF  // R=0,   G=0,   B=255
#define COLOR_YELLOW  0xFFFF00  // R=255, G=255, B=0

#define SENSITIVITY 0.004f

typedef struct s_dda {
    float   pos_x;
    float   pos_y;
    float   dir_x;
    float   dir_y;
    float   plane_x;
    float   plane_y;
    float   cam_x;
    float   ray_dir_x;
    float   ray_dir_y;
    int     map_x;
    int     map_y;
    float   side_dist_x;
    float   side_dist_y;
    float   delta_dist_x;
    float   delta_dist_y;
    int     step_x;
    int     step_y;
    int     wall_side;
    float   perp_wall_dist;
} t_dda;

typedef struct s_config
{
    char	*no_tex;
    char	*so_tex;
    char	*we_tex;
    char	*ea_tex;
    int		floor[3];
    int		ceil[3];
    char	**map;
    char    **original_map; 
    int		map_h;
    int		map_w;
    int		player_x;
    int		player_y;
    char	player_dir;
    char    *door_tex;    
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

typedef struct s_door
{
    int	x;
    int	y;
    int	open;  
}	t_door;

typedef struct s_doors
{
    t_door	*data;
    int		count;
    int		capacity;
}	t_doors;

int		was_door_before(int x, int y, char **original_map);
void	toggle_door(char **map, char **original_map,
            double player_x, double player_y, double dir_x, double dir_y);
int		pos_is_door(char **map, double px, double py);

void	doors_init(t_doors *doors);
void	doors_free(t_doors *doors);
int		doors_add(t_doors *doors, int x, int y);
int		doors_remove(t_doors *doors, int x, int y);
int		doors_is_saved(t_doors *doors, int x, int y);

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

typedef struct s_game
{
    void        *img;           // Image buffer pointer
    char        *img_addr;      // Image data address
    int         img_bpp;        // Bits per pixel
    int         img_line_len;   // Line length in bytes
    int         img_endian;     // Endian

    void        *mlx;
    void        *window;
    int         win_w;          /* changed from float to int */
    int         win_h;          /* changed from float to int */
    t_doors     doors;          /* changed: use t_doors (state) */
    t_player    player;
    t_config    *config;
    t_mouse     mouse;
    t_texture   no_texture;     // North wall texture
    t_texture   so_texture;     // South wall texture
    t_texture   we_texture;     // West wall texture
    t_texture   ea_texture;     // East wall texture
    t_texture   door_texture; /* <--- added: door texture */
}   t_game;
/// move player

void update_display(t_game *game, t_config *config);
void draw_mini_and_rays(t_game *game, t_config *config);
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

//PARSING FUNCTIONS 
int		has_cub_extension(char *path);
int		is_empty_line(char *line);
int		is_player_char(char c);
int		parse_file(char *filename, t_config *config);
void	parse_rgb(int color[3], char *s);
void	parse_identifier(t_config *cfg, char *line);
int		is_map_line(char *line);
char	**add_line_to_array(char **array, const char *line);
void	print_error(int code);
void	validate_map(t_config *config);

int	mouse_move(int x, int y, t_game *game);
int	mouse_release(int button, int x, int y, t_game *game);
int	mouse_press(int button, int x, int y, t_game *game);

int ft_isdigit(int c);
void    print_error_path(int code, const char *detail);
void	free_split_safe(char **arr);
void creat_window(t_game *game, t_config *config);

int was_door_before(int x, int y, char **original_map);
void free_config(t_config *config);
char **duplicate_map(char **map);
t_door *find_door(t_doors *doors, int x, int y);

#endif
