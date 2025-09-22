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

static void clear_screen(t_game *game)
{
    for (int y = 0; y < game->win_h; y++)
    {
        for (int x = 0; x < game->win_w; x++)
        {
            mlx_pixel_put(game->mlx, game->window, x, y, 0x000000);
        }
    }
}

static void draw_vertical_strip(t_game *game, int x, int start, int end, int color, int slice_width)
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

static void draw_rays(t_game *game, t_config *config)
{
    int r;
    float half_fov = (FOV_DEG / 2.0f);
    float start_angle_deg = (game->player.angle * 180.0f / M_PI) - half_fov;
    float step = FOV_DEG / (float)RAYS;

    int view_3d_width = game->win_w; // Use full screen width
    int slice_width = view_3d_width / RAYS; // Calculate slice width based on full screen width
    int view_3d_start_x = 0; // Start at the beginning of the screen

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

static void update_display(t_game *game, t_config *config)
{
    clear_screen(game);
    draw_rays(game, config); // Draw 3D view first (fullscreen)
    draw_mini_map(game, config); // Draw minimap on top as overlay
}

static int is_valid_position(t_config *config, float x, float y)
{
    int map_x = (int)(x / TILE);
    int map_y = (int)(y / TILE);
    
    float padding = 5.0f;
    int map_x_left = (int)((x - padding) / TILE);
    int map_x_right = (int)((x + padding) / TILE);
    int map_y_top = (int)((y - padding) / TILE);
    int map_y_bottom = (int)((y + padding) / TILE);
    
    return (map_at(config, map_x, map_y) != '1' &&
            map_at(config, map_x_left, map_y) != '1' &&
            map_at(config, map_x_right, map_y) != '1' &&
            map_at(config, map_x, map_y_top) != '1' &&
            map_at(config, map_x, map_y_bottom) != '1');
}

static void move_player(t_game *game, t_config *config, int direction)
{
    float move_speed = 3.0f;
    float new_x = game->player.x;
    float new_y = game->player.y;

    if (direction == 'w')
    {
        new_x += cosf(game->player.angle) * move_speed;
        new_y += sinf(game->player.angle) * move_speed;
    }
    else if (direction == 's')
    {
        new_x -= cosf(game->player.angle) * move_speed;
        new_y -= sinf(game->player.angle) * move_speed;
    }
    else if (direction == 'a')
    {
        new_x += cosf(game->player.angle - M_PI / 2) * move_speed;
        new_y += sinf(game->player.angle - M_PI / 2) * move_speed;
    }
    else if (direction == 'd')
    {
        new_x += cosf(game->player.angle + M_PI / 2) * move_speed;
        new_y += sinf(game->player.angle + M_PI / 2) * move_speed;
    }

    if (is_valid_position(config, new_x, game->player.y))
        game->player.x = new_x;
    if (is_valid_position(config, game->player.x, new_y))
        game->player.y = new_y;
}

static void rotate_player(t_game *game, int direction)
{
    float rotation_speed = 0.1f;
    
    if (direction == 1) // Right
        game->player.angle += rotation_speed;
    else if (direction == -1) // Left
        game->player.angle -= rotation_speed;
    
    if (game->player.angle < 0)
        game->player.angle += 2 * M_PI;
    if (game->player.angle >= 2 * M_PI)
        game->player.angle -= 2 * M_PI;
}

static int key_press(int keycode, t_game *game)
{
    t_config *config = game->config;
    
    switch (keycode)
    {
        case 13: // W key (forward)
        case 119: // w key
            move_player(game, config, 'w');
            break;
        case 0: // A key (left strafe)
        case 97: // a key
            move_player(game, config, 'a');
            break;
        case 1: // S key (backward)
        case 115: // s key
            move_player(game, config, 's');
            break;
        case 2: // D key (right strafe)
        case 100: // d key
            move_player(game, config, 'd');
            break;
        case 123: // Left arrow (rotate left)
            rotate_player(game, -1);
            break;
        case 124: // Right arrow (rotate right)
            rotate_player(game, 1);
            break;
        case 53: // ESC key (exit)
            mlx_destroy_window(game->mlx, game->window);
            exit(0);
            break;
    }
    
    update_display(game, config);
    return (0);
}

static int close_window(t_game *game)
{
    mlx_destroy_window(game->mlx, game->window);
    exit(0);
    return (0);
}

void draw_mini_and_rays(t_game *game, t_config *config)
{
    clear_screen(game);
    draw_rays(game, config); // Draw 3D view first (fullscreen)
    draw_mini_map(game, config); // Draw minimap on top as overlay
}

void creat_window(t_game *game, t_config *config)
{
    game->win_w = 1200; // Standard width for better 3D experience
    game->win_h = 800;  // Standard height for better 3D experience

    game->config = config;

    game->mlx = mlx_init();
    game->window = mlx_new_window(game->mlx,
                                  game->win_w,
                                  game->win_h,
                                  "Cub3D - WASD to move, Arrows to rotate");

    init_player_from_config(game, config);
    
    mlx_hook(game->window, 2, 1L<<0, key_press, game); // Key press
    mlx_hook(game->window, 17, 1L<<17, close_window, game); // Window close
    
    draw_mini_and_rays(game, config);
    
    mlx_loop(game->mlx);
}