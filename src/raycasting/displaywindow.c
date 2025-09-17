// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   displaywindow.c                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ahabibi- <ahabibi-@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/09/15 11:27:09 by ahabibi-          #+#    #+#             */
// /*   Updated: 2025/09/16 18:00:49 by ahabibi-         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/cub.h"

// void draw_pixel(t_game *game, int i, int j, int color)
// {
//     int x, y;
//     x =0;
//     y =0;

//     while(x < 8)
//     {
//         y=0;
//         while(y < 8)
//         {
//             mlx_pixel_put(game->mlx, game->window, i * 8 + x + 30, j * 8 + y + 30, color);
//             y++;
//         }
//         x++;
//     }
// }
// void draw_rays(t_game *game, t_config *config)
// {
//     int r;
//     for (r = 0; r < 60; r++)  // 60 rays = FOV
//     {
//         float ray_angle = game->player.angle - 30 + r; // -30° to +30°
//         float ray_x = game->player.x;
//         float ray_y = game->player.y;

//         while (1)
//         {
//             int map_x = (int)(ray_x / 8);
//             int map_y = (int)(ray_y / 8);

//             if (config->map[map_y][map_x] == '1') {
//                 mlx_pixel_put(game->mlx, game->window, ray_x, ray_y, COLOR_BLUE);
//                 break;
//             }
//             ray_x += cos(ray_angle * M_PI / 180);
//             ray_y += sin(ray_angle * M_PI / 180);

//             mlx_pixel_put(game->mlx, game->window, ray_x, ray_y, COLOR_PINK);
//         }
//     }
// }
// void draw_mini_map(t_game *game,t_config *config)
// {
//   int i;
//   int j;
//   i =0;
//   while(config->map[i])
//   {
//     j =0;
//     while(config->map[i][j])
//         {
//             // if(config->map[i][j] == '1')
//             //     draw_pixel(game,j,i,COLOR_WHITE);
//             if(config->map[i][j] == '0')
//                 draw_pixel(game,j,i,COLOR_YELLOW);
//             if(is_player_char(config->map[i][j]))
//                 draw_pixel(game,j,i,COLOR_PINK);
//             j++;
//         }
//         i++;
//     }
// }
// // void handle_key(t_game *game)
// // {
    
// // }
// void creat_window(t_game *game,t_config *config)
// {
//     game->mlx = mlx_init();
//     game->window = mlx_new_window(game->mlx,config->map_w * 32,config->map_h * 32 ,"FUCKING_CUBE3D");
//     draw_mini_map(game,config);   
//     draw_rays(game,config);
//     // mlx_key_hook(game->window, handle_key, game);
// 	// mlx_hook(game->window, 17, 0, NULL, game);
// 	mlx_loop(game->mlx);
// }


// #include "../../includes/cub.h"
// #include <math.h>

// #ifndef M_PI
// # define M_PI 3.14159265358979323846
// #endif

// void draw_pixel(t_game *game, int i, int j, int color)
// {
//     int x, y;
//     x = 0;
//     while (x < 8)
//     {
//         y = 0;
//         while (y < 8)
//         {
//             mlx_pixel_put(game->mlx, game->window,
//                 i * 8 + x + 30, j * 8 + y + 30, color);
//             y++;
//         }
//         x++;
//     }
// }

// void draw_mini_map(t_game *game, t_config *config)
// {
//     int i = 0, j;
//     while (config->map[i])
//     {
//         j = 0;
//         while (config->map[i][j])
//         {
//             if (config->map[i][j] == '1')
//                 draw_pixel(game, j, i, COLOR_WHITE);
//             if (config->map[i][j] == '0')
//                 draw_pixel(game, j, i, COLOR_YELLOW);
//             if (is_player_char(config->map[i][j]))
//                 draw_pixel(game, j, i, COLOR_PINK);
//             j++;
//         }
//         i++;
//     }
// }

// void draw_rays(t_game *game, t_config *config)
// {
//     int r;
//     for (r = 0; r < 90; r++)
//     {
//         float ray_angle = (game->player.angle - 30 + r) * M_PI / 180.0;
//         float ray_x = game->player.x;
//         float ray_y = game->player.y;
//         float dist = 0;

//         while (1)
//         {
//         int map_x = (int)(ray_x / 8);
//         int map_y = (int)(ray_y / 8);
//         if (config->map[map_y][map_x] == '1')
//             break;
//         ray_x += cos(ray_angle);
//         ray_y += sin(ray_angle);
//         dist++;

//         mlx_pixel_put(game->mlx, game->window, ray_x + 30, ray_y + 30, COLOR_BLUE);
//         }

//         int line_height = (int)(2000 / (dist + 1));
//         if (line_height > 320) line_height = 320;

//         int start = 160 - line_height / 2;
//         int end   = 160 + line_height / 2;

//         int screen_x = r * 8 + 400;
//         int y;
//         for (y = start; y < end; y++)
//             mlx_pixel_put(game->mlx, game->window, screen_x, y, COLOR_WHITE);
//     }
// }

// void creat_window(t_game *game, t_config *config)
// {
//     game->mlx = mlx_init();
//     game->window = mlx_new_window(game->mlx,config->map_w * 32 + 600,config->map_h * 32,"FUCKING CUB3D");

//     game->player.x = 9 * 8 + 4;
//     game->player.y = 9 * 8 + 4;
//     game->player.angle = 0;

//     draw_mini_map(game, config);
//     draw_rays(game, config);

//     mlx_loop(game->mlx);
// }
#include "../../includes/cub.h"

static void draw_pixel(t_game *game, int i, int j, int color)
{
    int x, y;
    x = 0;
    while (x < TILE)
    {
        y = 0;
        while (y < TILE)
        {
            mlx_pixel_put(game->mlx, game->window,
                          i * TILE + x + MINIMAP_OX,
                          j * TILE + y + MINIMAP_OY,
                          color);
            y++;
        }
        x++;
    }
}

static void draw_mini_map(t_game *game, t_config *config)
{
    int i = 0, j;
    while (config->map[i])
    {
        j = 0;
        while (config->map[i][j])
        {
            if (config->map[i][j] == '1')
                draw_pixel(game, j, i, COLOR_WHITE);
            else
                draw_pixel(game, j, i, COLOR_YELLOW);
            j++;
        }
        i++;
    }
    int p_mx = (int)roundf(game->player.x);
    int p_my = (int)roundf(game->player.y);
    int px = p_mx + MINIMAP_OX;
    int py = p_my + MINIMAP_OY;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            mlx_pixel_put(game->mlx, game->window, px + dx, py + dy, COLOR_PINK);
}

static int map_at(t_config *cfg, int mx, int my)
{
    if (my < 0 || mx < 0)
        return '1';
    if (my >= cfg->map_h || mx >= cfg->map_w)
        return '1';
    if (!cfg->map[my])
        return '1';
    if (!cfg->map[my][mx])
        return '1';
    return cfg->map[my][mx];
}

static void init_player_from_config(t_game *game, t_config *config)
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

void draw_rays(t_game *game, t_config *config)
{
    int r;
    float half_fov = (FOV_DEG / 2.0f);
    float start_angle_deg = (game->player.angle * 180.0f / M_PI) - half_fov;
    float step = FOV_DEG / (float)RAYS;
    for (r = 0; r < RAYS; r++)
    {
        float ray_angle = (start_angle_deg + r * step) * M_PI / 180.0f;
        float ray_x = game->player.x;
        float ray_y = game->player.y;
        const float STEP = 0.5f;
        float dist = 0.0f;
        int hit = 0;
        int hit_mx = 0, hit_my = 0;
        while (!hit && dist < 2000.0f)
        {
            int map_x = (int)(ray_x / TILE);
            int map_y = (int)(ray_y / TILE);
            if (map_at(config, map_x, map_y) == '1')
            {
                hit = 1;
                hit_mx = map_x;
                hit_my = map_y;
                break;
            }
            ray_x += cosf(ray_angle) * STEP;
            ray_y += sinf(ray_angle) * STEP;
            dist += STEP;
            mlx_pixel_put(game->mlx, game->window,
                          (int)roundf(ray_x) + MINIMAP_OX,
                          (int)roundf(ray_y) + MINIMAP_OY,
                          COLOR_PINK);
        }
        if (hit)
        {
            mlx_pixel_put(game->mlx, game->window,
                          (int)roundf(ray_x) + MINIMAP_OX,
                          (int)roundf(ray_y) + MINIMAP_OY,
                          COLOR_BLUE);
        }
        float dist_world_pixels = dist;
        if (dist_world_pixels <= 0.0001f)
            dist_world_pixels = 0.0001f;
        int line_height = (int)((TILE * 2000.0f) / (dist_world_pixels + 1.0f));
        if (line_height > MAX_SLICE_HEIGHT)
            line_height = MAX_SLICE_HEIGHT;
        int start = 160 - line_height / 2;
        int end = 160 + line_height / 2;
        int screen_x = r * 8 + (config->map_w * TILE) + 100;
        int y;
        for (y = start; y < end; y++)
            mlx_pixel_put(game->mlx, game->window, screen_x, y, COLOR_WHITE);
    }
}

void draw_mini_and_rays(t_game *game, t_config *config)
{
    draw_mini_map(game, config);
    draw_rays(game, config);
}

void creat_window(t_game *game, t_config *config)
{
    int win_w = config->map_w * TILE + 600;
    int win_h = config->map_h * TILE + 100;
    game->mlx = mlx_init();
    game->window = mlx_new_window(game->mlx, win_w, win_h, "MiniMap + Raycast");
    init_player_from_config(game, config);
    draw_mini_and_rays(game, config);
    mlx_loop(game->mlx);
}
