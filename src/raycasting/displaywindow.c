#include "../../includes/cub.h"

void draw_pixel(t_game *game, int i, int j, int color)
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

void draw_mini_map(t_game *game, t_config *config)
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

int map_at(t_config *cfg, int mx, int my)
{
    if (my < 0 || mx < 0)
        return '1';
    if (my > cfg->map_h || mx > cfg->map_w)
        return '1';
    if (!cfg->map[my])
        return '1';
    if (!cfg->map[my][mx])
        return '1';
    return cfg->map[my][mx];
}

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

void clear_screen(t_game *game)
{
    for (int y = 0; y < game->win_h; y++)
    {
        for (int x = 0; x < game->win_w; x++)
        {
            mlx_pixel_put(game->mlx, game->window, x, y, 0x000000);
        }
    }
}

void draw_vertical_strip(t_game *game, int x, int start, int end, int color, int slice_width)
{
    if (x < 0 || x >= game->win_w)
        return;
    
    for (int dx = 0; dx < slice_width && (x + dx) < game->win_w; dx++)
    {
        for (int y = start; y < end; y++)
        {
            if (y >= 0 && y < game->win_h)
                mlx_pixel_put(game->mlx, game->window, x + dx, y, color);
        }
    }
}

void draw_rays(t_game *game, t_config *config)
{
    int r;
    float half_fov = (FOV_DEG / 2.0f);
    float start_angle_deg = (game->player.angle * 180.0f / M_PI) - half_fov;
    float step = FOV_DEG / (float)RAYS;

    int view_3d_width = game->win_w;
    int slice_width = view_3d_width / RAYS;
    int view_3d_start_x = 0;

    for (r = 0; r < RAYS; r++)
    {
        float ray_angle = (start_angle_deg + r * step) * M_PI / 180.0f;
        float ray_x = game->player.x;
        float ray_y = game->player.y;
        const float STEP = 0.5f;
        float dist = 0.0f;
        int hit = 0;

        while (!hit && dist < 2000.0f)
        {
            int map_x = (int)(ray_x / TILE);
            int map_y = (int)(ray_y / TILE);
            if (map_at(config, map_x, map_y) == '1')
            {
                hit = 1;
                break;
            }
            ray_x += cosf(ray_angle) * STEP;
            ray_y += sinf(ray_angle) * STEP;
            dist += STEP;
        }

        float corrected_dist = dist * cosf(ray_angle - game->player.angle);
        if (corrected_dist <= 0.0001f)
            corrected_dist = 0.0001f;

        float projection_distance = (game->win_h / 2.0f) / tanf(FOV_DEG * M_PI / 360.0f);
        int line_height = (int)((TILE * projection_distance) / corrected_dist);
        
        if (line_height > game->win_h * 2)
            line_height = game->win_h * 2;

        int start = (game->win_h / 2) - (line_height / 2);
        int end   = (game->win_h / 2) + (line_height / 2);
        
        if (start < 0) start = 0;
        if (end > game->win_h) end = game->win_h;

        int screen_x = view_3d_start_x + (r * slice_width);

        if (screen_x < game->win_w)
        {
            draw_vertical_strip(game, screen_x, 0, start, COLOR_CEILING, slice_width);

            draw_vertical_strip(game, screen_x, start, end, COLOR_WALL, slice_width);

            draw_vertical_strip(game, screen_x, end, game->win_h, COLOR_FLOOR, slice_width);
        }
    }
}

void update_display(t_game *game, t_config *config)
{
    draw_rays(game, config);
    draw_mini_map(game, config);
}

int close_window(t_game *game)
{
    mlx_clear_window(game->mlx,game->window);
    mlx_destroy_window(game->mlx, game->window);
    exit(0);
    return (0);
}

void draw_mini_and_rays(t_game *game, t_config *config)
{
    draw_rays(game, config);
    draw_mini_map(game, config);
}

void creat_window(t_game *game, t_config *config)
{
    game->win_w = 1200;
    game->win_h = config->map_h * TILE > 600 ? config->map_h * TILE : 600;

    game->config = config;

    game->mlx = mlx_init();
    game->window = mlx_new_window(game->mlx,
                                  game->win_w,
                                  game->win_h,
                                  "FUCKING CUB3D");

    init_player_from_config(game, config);
    
    mlx_hook(game->window, 2, 1L<<0, key_press, game);
    mlx_hook(game->window, 17, 1L<<17, close_window, game);
    
    draw_mini_and_rays(game, config);
    
    mlx_loop(game->mlx);
}