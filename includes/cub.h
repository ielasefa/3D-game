// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   cub.h                                              :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/09/08 11:49:59 by iel-asef          #+#    #+#             */
// /*   Updated: 2025/10/18 02:08:25 by iel-asef         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #ifndef CUB3D_H
// # define CUB3D_H
// #ifndef M_PI
// # define M_PI 3.14159265358979323846
// #endif

// # include <fcntl.h>
// # include <stdlib.h>
// # include <unistd.h>
// # include <stdio.h>
// # include <limits.h>
// # include "get_next_line.h"
// # include "libft.h"
// # include "minilibx-linux/mlx.h"
// # include <math.h>
// /* ERRORS */
// # define ERR_INVALID_PATH    1
// # define ERR_INVALID_EXT     2
// # define ERR_INVALID_RGB     3
// # define ERR_INVALID_MAP     4
// # define ERR_MISSING_TEXTURE 5
// # define ERR_MISSING_COLOR   6
// # define ERR_NO_PLAYER       7
// # define ERR_MULTIPLAYER     8
// # define ERR_UNKNOWN         99

// #define TILE 8
// #define MINIMAP_OX 30
// #define MINIMAP_OY 30
// #define FOV_DEG 60
// #define RAYS 90
// #define MAX_SLICE_HEIGHT 320
// #define COLOR_CEILING 0x87CEEB  // sky blue
// #define COLOR_FLOOR   0x228B22  // forest green
// #define COLOR_WALL    0xFFD700  // gold (current walls)

// /* ERROR MESSAGES */
// # define INVALID_PATH        "Error\nInvalid file path"
// # define INVALID_EXT         "Error\nInvalid file extension"
// # define USAGE_ERR           "Error\nUsage: ./cub3D <file.cub>"
// // colors
// #define COLOR_WHITE   0xFFFFFF  // R=255, G=255, B=255
// #define COLOR_PINK    0xFF66B2  // R=255, G=102, B=178
// #define COLOR_BLUE    0x0000FF  // R=0,   G=0,   B=255
// #define COLOR_YELLOW  0xFFFF00  // R=255, G=255, B=0

// #define SENSITIVITY 0.001f

// typedef struct s_dda {
//     float   pos_x;
//     float   pos_y;
//     float   dir_x;
//     float   dir_y;
//     float   plane_x;
//     float   plane_y;
//     float   cam_x;
//     float   ray_dir_x;
//     float   ray_dir_y;
//     int     map_x;
//     int     map_y;
//     float   side_dist_x;
//     float   side_dist_y;
//     float   delta_dist_x;
//     float   delta_dist_y;
//     int     step_x;
//     int     step_y;
//     int     wall_side;
//     float   perp_wall_dist;
// } t_dda;

// typedef struct s_config
// {
//     char    *hand_texture;
//     char	*no_tex;
//     char	*so_tex;
//     char	*we_tex;
//     char	*ea_tex;
//     int		floor[3];
//     int		ceil[3];
//     char	**map;
//     int		map_h;
//     int		map_w;
//     int		player_x;
//     int		player_y;
//     char	player_dir;
// }	t_config;

// typedef struct s_hands_pixel
// {
//     int            screen_x;
//     int            screen_y;
// }                t_hands_pixel;
// typedef struct s_texture {
//     void    *img;
//     char    *addr;
//     int     width;
//     int     height;
//     int     bits_per_pixel;
//     int     line_length;
//     int     endian;
// }   t_texture;

// typedef struct s_mouse
// {
//     int x;
//     int y;
//     int prev_x;
//     int prev_y;
//     int is_pressed;
// }   t_mouse;

// typedef struct s_player {
// float x;    
// float y;     
// float angle;  
// }   t_player;

// typedef struct s_game {
//     int        stop;
//     void        *img;           // Image buffer pointer
//     char        *img_addr;      // Image data address
//     int         img_bpp;        // Bits per pixel
//     int         img_line_len;   // Line length in bytes
//     int         img_endian;     // Endian
//     int        hands_anim_frame;
//     void        *mlx;
//     void        *window;
//     float       win_w;
//     float       win_h;
//     t_player    player;
//     t_config    *config;
//     t_mouse      mouse;
//     t_texture   hand_texture;
//     t_texture   no_texture;     // North wall texture
//     t_texture   so_texture;     // South wall texture
//     t_texture   we_texture;     // West wall texture
//     t_texture   ea_texture;     // East wall texture
// }   t_game;
// /// move player

// void update_display(t_game *game, t_config *config);
// void draw_mini_and_rays(t_game *game, t_config *config);
// int close_window(t_game *game);
// void update_display(t_game *game, t_config *config);
// void draw_rays(t_game *game, t_config *config);
// void draw_vertical_strip(t_game *game, int x, int start, int end, int color, int slice_width);
// void clear_screen(t_game *game);
// void init_player_from_config(t_game *game, t_config *config);
// int map_at(t_config *cfg, int mx, int my);
// void draw_mini_map(t_game *game, t_config *config);
// void draw_pixel(t_game *game, int i, int j, int color);

// int is_valid_position(t_config *config, float x, float y);
// void move_player(t_game *game, t_config *config, int direction);
// // int (t_config *config, float x, float y);
// void rotate_player(t_game *game, int direction);

// // window displays

// void creat_window(t_game *game,t_config *config);
// int key_press(int keycode, t_game *game);

// //PARSING FUNCTIONS 
// int		has_cub_extension(char *path);
// int		is_empty_line(char *line);
// int		is_player_char(char c);
// int		parse_file(char *filename, t_config *config);
// void	parse_rgb(int color[3], char *s);
// void	parse_identifier(t_config *cfg, char *line);
// int		is_map_line(char *line);
// char	**add_line_to_array(char **array, char *line);
// void	print_error(int code);
// void	validate_map(t_config *config);

// int	mouse_move(int x, int y, t_game *game);
// int	mouse_release(int button, int x, int y, t_game *game);
// int	mouse_press(int button, int x, int y, t_game *game);

// #endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:49:59 by iel-asef          #+#    #+#             */
/*   Updated: 2025/10/18 02:08:25 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif
# include <sys/time.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include <math.h>
# include "get_next_line.h"
# include "libft.h"
# include "minilibx-linux/mlx.h"

/* ERROR CODES */
# define ERR_INVALID_PATH    1
# define ERR_INVALID_EXT     2
# define ERR_INVALID_RGB     3
# define ERR_INVALID_MAP     4
# define ERR_MISSING_TEXTURE 5
# define ERR_MISSING_COLOR   6
# define ERR_NO_PLAYER       7
# define ERR_MULTIPLAYER     8
# define ERR_UNKNOWN         99

/* MAP & RENDERING */
# define MINIMAP_SCALE       8
# define TILE                8
# define MINIMAP_OX          30
# define MINIMAP_OY          30
# define FOV_DEG             60
# define RAYS                90
# define MAX_SLICE_HEIGHT    320

/* COLORS */
# define COLOR_CEILING       0x87CEEB  // sky blue
# define COLOR_FLOOR         0x228B22  // forest green
# define COLOR_WALL          0xFFD700  // gold
# define COLOR_WHITE         0xFFFFFF  // R=255, G=255, B=255
# define COLOR_PINK          0xFF66B2  // R=255, G=102, B=178
# define COLOR_BLUE          0x0000FF  // R=0,   G=0,   B=255
# define COLOR_YELLOW        0xFFFF00  // R=255, G=255, B=0

/* CONTROLS */
# define SENSITIVITY         0.004f

/* ERROR MESSAGES */
# define INVALID_PATH        "Error\nInvalid file path"
# define INVALID_EXT         "Error\nInvalid file extension"
# define USAGE_ERR           "Error\nUsage: ./cub3D <file.cub>"

/* ========== STRUCTURES ========== */

typedef struct s_dda {
	float	pos_x;
	float	pos_y;
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
	float	cam_x;
	float	ray_dir_x;
	float	ray_dir_y;
	int		map_x;
	int		map_y;
	float	side_dist_x;
	float	side_dist_y;
	float	delta_dist_x;
	float	delta_dist_y;
	int		step_x;
	int		step_y;
	int		wall_side;
	float	perp_wall_dist;
}	t_dda;

typedef struct s_config
{
	char	*no_tex;
	char	*so_tex;
	char	*we_tex;
	char	*ea_tex;
	char	*door_tex;
	char	*hand_texture;
	char	*g_gnl_last_line;
	int		floor[3];
	int		ceil[3];
	char	**map;
	char	**original_map;
	int		map_h;
	int		map_w;
	int		player_x;
	int		player_y;
	char	player_dir;
}	t_config;

typedef struct s_texture {
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

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

typedef struct s_hands_pixel
{
	int	screen_x;
	int	screen_y;

}	t_hands_pixel;

typedef struct s_mouse
{
	int	x;
	int	y;
	int	prev_x;
	int	prev_y;
	int	is_pressed;
}	t_mouse;

typedef struct s_player {
	float	x;
	float	y;
	float   prev_x;
    float   prev_y;
	float	angle;
}	t_player;
typedef struct s_game
{
	unsigned int last_frame_time;
	void		*img;
	char		*img_addr;
	int			img_bpp;
	int			img_line_len;
	int			img_endian;
	int			hands_anim_frame;
	void		*mlx;
	void		*window;
	int			win_w;
	int			win_h;
	t_doors		doors;
	t_player	player;
	t_config	*config;
	t_mouse		mouse;
	t_texture	hand_texture[12];
	t_texture	no_texture;
	t_texture	so_texture;
	t_texture	we_texture;
	t_texture	ea_texture;
	t_texture	door_texture;
}	t_game;

/* ========== FUNCTION DECLARATIONS ========== */
int select_texture(t_dda *dda);

/* Display & Window */
void	update_display(t_game *game, t_config *config);
void	draw_mini_and_rays(t_game *game, t_config *config);
void	draw_rays(t_game *game, t_config *config);
void	draw_vertical_strip(t_game *game, int x, int start, int end,
			int color, int slice_width);
void	draw_textured_vertical_strip(t_game *game, int x, int start, int end,
			t_texture *tex, int tex_x, int slice_width);
void	clear_screen(t_game *game);
void	draw_mini_map(t_game *game, t_config *config);
void	draw_pixel(t_game *game, int i, int j, int color);
void	creat_window(t_game *game, t_config *config);

/* Raycasting */
void	init_raycasting(t_game *game, t_dda *dda, int x);
void	get_delta_dist(t_dda *dda);
void	get_step_and_sidedist(t_dda *dda);
void	digital_differential_analyser(t_config *config, t_dda *dda);
void	draw_column(t_game *game, t_config *config, t_dda *dda, int x);
int		map_at(t_config *cfg, int mx, int my);

/* Player */
void	init_player_from_config(t_game *game, t_config *config);
int		is_valid_position(t_config *config, float x, float y);
void	move_player(t_game *game, t_config *config, int direction);
void	rotate_player(t_game *game, int direction);

/* Hand/Weapon Drawing */
void	draw_hands(t_game *game);

/* Textures */
void	load_textures(t_game *game, t_config *config);

/* Input Events */
int		key_press(int keycode, t_game *game);
int		mouse_move(int x, int y, t_game *game);
int		mouse_release(int button, int x, int y, t_game *game);
int		mouse_press(int button, int x, int y, t_game *game);
int		close_window(t_game *game);

/* Door System */
void	doors_init(t_doors *doors);
void	doors_free(t_doors *doors);
void	doors_build(t_doors *doors, char **map, int h, int w);
t_door	*find_door(t_doors *doors, int x, int y);
void	toggle_door_in_front(t_config *cfg, t_doors *doors,
			double px, double py, double ang);
int		was_door_before(int x, int y, char **original_map);
void	toggle_door(char **map, char **original_map,
			double player_x, double player_y, double dir_x, double dir_y);
int		pos_is_door(char **map, double px, double py);

/* Parsing */
int		has_cub_extension(char *path);
int		is_empty_line(char *line);
int		is_player_char(char c);
int	parse_file(char *filename, t_config *config);
int	parse_rgb(int color[3], char *s);
void	parse_identifier(t_config *cfg, char *line);
int	is_map_line(char *line);
char	**add_line_to_array(char **array, const char *line);
int	is_valid_map_char(char c);
int	get_height(char **map);
void	print_error(int code);
void	print_error_path(int code, const char *detail);
void	validate_map(t_config *config);

/* Memory Management */
void	free_config(t_config *config);
void	free_split_safe(char **arr);
char	**duplicate_map(char **map);
void    cleanup_and_exit(t_game *game);

/* Libft Utils (if not in libft.h) */
int		ft_isdigit(int c);
void gnl_cleanup1(t_config *config);

#endif