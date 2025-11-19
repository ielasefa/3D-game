#include "../includes/cub.h"

void init_player_from_config(t_game *game, t_config *config)
{
    game->player.x = config->player_x * TILE + TILE / 2.0f;
    game->player.y = config->player_y * TILE + TILE / 2.0f;
    if (config->player_dir == 'N')
        game->player.angle = -M_PI / 2.0f;
    else if (config->player_dir == 'S')
        game->player.angle = M_PI / 2.0f;
    else if (config->player_dir == 'W')
        game->player.angle = M_PI;
    else
        game->player.angle = 0.0f;
}


void init_raycasting(t_game *game, t_dda *dda, int x)
{
    dda->pos_x = game->player.x / TILE;
    dda->pos_y = game->player.y / TILE;
    
    dda->dir_x = cosf(game->player.angle);
    dda->dir_y = sinf(game->player.angle);
    
    float fov_rad = FOV_DEG * M_PI / 180.0f;
    float plane_len = tanf(fov_rad / 2.0f);
    dda->plane_x = -dda->dir_y * plane_len;
    dda->plane_y = dda->dir_x * plane_len;
    
    dda->cam_x = 2 * x / (float)game->win_w - 1;
    
    dda->ray_dir_x = dda->dir_x + dda->plane_x * dda->cam_x;
    dda->ray_dir_y = dda->dir_y + dda->plane_y * dda->cam_x;
    
    dda->map_x = (int)dda->pos_x;
    dda->map_y = (int)dda->pos_y;
}

int select_texture(t_dda *dda)
{
    int texture = 0;
    
    if (dda->wall_side == 1 && dda->ray_dir_y < 0)
        texture = 1; // SOUTH
    else if (dda->wall_side == 1 && dda->ray_dir_y > 0)
        texture = 0; // NORTH
    else if (dda->wall_side == 0 && dda->ray_dir_x < 0)
        texture = 2; // WEST
    else if (dda->wall_side == 0 && dda->ray_dir_x > 0)
        texture = 3; // EAST
    
    return texture;
}