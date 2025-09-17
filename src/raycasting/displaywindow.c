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
