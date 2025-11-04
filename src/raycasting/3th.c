#include "../includes/cub.h"
#include <string.h>

static int rgb_to_int(int rgb[3])
{
    return (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
}

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
                draw_pixel(game, j, i, COLOR_BLUE);
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
    if (my >= cfg->map_h || mx >= cfg->map_w)
        return '1';
    if (!cfg->map[my])
        return '1';
    if ((int)ft_strlen(cfg->map[my]) <= mx)
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

int get_texture_pixel(t_texture *tex, int x, int y)
{
    if (!tex || !tex->addr || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return COLOR_WALL;
    
    char *pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return *(int *)pixel;
}

void draw_textured_vertical_strip(t_game *game, int x, int start, int end, 
                                   t_texture *tex, int tex_x, int slice_width)
{
    if (x < 0 || x >= game->win_w)
        return;
    
    if (!tex || !tex->addr)
    {
        draw_vertical_strip(game, x, start, end, COLOR_WALL, slice_width);
        return;
    }
    
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= tex->width) tex_x = tex->width - 1;
    
    int wall_height = end - start;
    if (wall_height <= 0) return;
    
    float step = (float)tex->height / (float)wall_height;
    float tex_pos = (start - game->win_h / 2 + wall_height / 2) * step;
    
    for (int dx = 0; dx < slice_width && (x + dx) < game->win_w; dx++)
    {
        float current_tex_pos = tex_pos;
        for (int y = start; y < end; y++)
        {
            if (y >= 0 && y < game->win_h)
            {
                int tex_y = (int)current_tex_pos % tex->height;
                if (tex_y < 0) tex_y = 0;
                if (tex_y >= tex->height) tex_y = tex->height - 1;
                
                int color = get_texture_pixel(tex, tex_x, tex_y);
                mlx_pixel_put(game->mlx, game->window, x + dx, y, color);
                
                current_tex_pos += step;
            }
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

// DDA raycasting structure
void init_raycasting(t_game *game, t_dda *dda, int x)
{
    // Convert player position from pixel to grid coordinates
    dda->pos_x = game->player.x / TILE;
    dda->pos_y = game->player.y / TILE;
    
    // Direction vector
    dda->dir_x = cosf(game->player.angle);
    dda->dir_y = sinf(game->player.angle);
    
    // Camera plane (perpendicular to direction, for FOV)
    float fov_rad = FOV_DEG * M_PI / 180.0f;
    float plane_len = tanf(fov_rad / 2.0f);
    dda->plane_x = -dda->dir_y * plane_len;
    dda->plane_y = dda->dir_x * plane_len;
    
    // X-coordinate in camera space (-1 to +1)
    dda->cam_x = 2 * x / (float)game->win_w - 1;
    
    // Ray direction
    dda->ray_dir_x = dda->dir_x + dda->plane_x * dda->cam_x;
    dda->ray_dir_y = dda->dir_y + dda->plane_y * dda->cam_x;
    
    // Current map position
    dda->map_x = (int)dda->pos_x;
    dda->map_y = (int)dda->pos_y;
}

void get_delta_dist(t_dda *dda)
{
    if (dda->ray_dir_x == 0)
        dda->delta_dist_x = 1e30;
    else
        dda->delta_dist_x = fabsf(1 / dda->ray_dir_x);
    
    if (dda->ray_dir_y == 0)
        dda->delta_dist_y = 1e30;
    else
        dda->delta_dist_y = fabsf(1 / dda->ray_dir_y);
}

void get_step_and_sidedist(t_dda *dda)
{
    if (dda->ray_dir_x < 0)
    {
        dda->step_x = -1;
        dda->side_dist_x = (dda->pos_x - dda->map_x) * dda->delta_dist_x;
    }
    else
    {
        dda->step_x = 1;
        dda->side_dist_x = (dda->map_x + 1.0 - dda->pos_x) * dda->delta_dist_x;
    }
    
    if (dda->ray_dir_y < 0)
    {
        dda->step_y = -1;
        dda->side_dist_y = (dda->pos_y - dda->map_y) * dda->delta_dist_y;
    }
    else
    {
        dda->step_y = 1;
        dda->side_dist_y = (dda->map_y + 1.0 - dda->pos_y) * dda->delta_dist_y;
    }
}

void digital_differential_analyser(t_config *config, t_dda *dda)
{
    int wall = 0;
    
    while (wall == 0)
    {
        if (dda->side_dist_x < dda->side_dist_y)
        {
            dda->side_dist_x += dda->delta_dist_x;
            dda->map_x += dda->step_x;
            dda->wall_side = 0;
        }
        else
        {
            dda->side_dist_y += dda->delta_dist_y;
            dda->map_y += dda->step_y;
            dda->wall_side = 1;
        }
        
        if (map_at(config, dda->map_x, dda->map_y) == '1')
            wall = 1;
    }
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

void draw_column(t_game *game, t_config *config, t_dda *dda, int x)
{
    if (dda->wall_side == 0)
        dda->perp_wall_dist = (dda->side_dist_x - dda->delta_dist_x);
    else
        dda->perp_wall_dist = (dda->side_dist_y - dda->delta_dist_y);
    
    if (dda->perp_wall_dist < 0.1f)
        dda->perp_wall_dist = 0.1f;
    
    int line_height = (int)(game->win_h / dda->perp_wall_dist);
    int draw_start = -line_height / 2 + game->win_h / 2;
    int draw_end = line_height / 2 + game->win_h / 2;
    
    if (draw_start < 0) draw_start = 0;
    if (draw_end >= game->win_h) draw_end = game->win_h - 1;
    
    float wall_x;
    if (dda->wall_side == 0)
        wall_x = dda->pos_y + dda->perp_wall_dist * dda->ray_dir_y;
    else
        wall_x = dda->pos_x + dda->perp_wall_dist * dda->ray_dir_x;
    wall_x -= floorf(wall_x);
    
    int tex_idx = select_texture(dda);
    t_texture *wall_tex = NULL;
    
    if (tex_idx == 0)
        wall_tex = &game->no_texture;
    else if (tex_idx == 1)
        wall_tex = &game->so_texture;
    else if (tex_idx == 2)
        wall_tex = &game->we_texture;
    else if (tex_idx == 3)
        wall_tex = &game->ea_texture;
    
    int tex_x = 0;
    if (wall_tex)
    {
        tex_x = (int)(wall_x * wall_tex->width);
        
        if ((dda->wall_side == 0 && dda->ray_dir_x > 0) || 
            (dda->wall_side == 1 && dda->ray_dir_y < 0))
        {
            tex_x = wall_tex->width - tex_x - 1;
        }
    }
    
    int ceiling_color = rgb_to_int(config->ceil);
    int floor_color = rgb_to_int(config->floor);
    
    draw_vertical_strip(game, x, 0, draw_start, ceiling_color, 1);
    draw_textured_vertical_strip(game, x, draw_start, draw_end, wall_tex, tex_x, 1);
    draw_vertical_strip(game, x, draw_end, game->win_h, floor_color, 1);
}

void draw_rays(t_game *game, t_config *config)
{
    t_dda dda;
    
    for (int x = 0; x < game->win_w; x++)
    {
        init_raycasting(game, &dda, x);
        
        get_delta_dist(&dda);
        
        get_step_and_sidedist(&dda);
        
        digital_differential_analyser(config, &dda);
        
        draw_column(game, config, &dda, x);
    }
}

void update_display(t_game *game, t_config *config)
{
    draw_rays(game, config);
    draw_mini_map(game, config);
}

int close_window(t_game *game)
{
    mlx_clear_window(game->mlx, game->window);
    mlx_destroy_window(game->mlx, game->window);
    exit(0);
    return (0);
}

void draw_mini_and_rays(t_game *game, t_config *config)
{
    draw_rays(game, config);
    draw_mini_map(game, config);
}

void load_textures(t_game *game, t_config *config)
{
    // Load North texture
    game->no_texture.img = mlx_xpm_file_to_image(game->mlx, config->no_tex,
                                                   &game->no_texture.width,
                                                   &game->no_texture.height);
    if (game->no_texture.img)
        game->no_texture.addr = mlx_get_data_addr(game->no_texture.img,
                                                   &game->no_texture.bits_per_pixel,
                                                   &game->no_texture.line_length,
                                                   &game->no_texture.endian);

    // Load South texture
    game->so_texture.img = mlx_xpm_file_to_image(game->mlx, config->so_tex,
                                                   &game->so_texture.width,
                                                   &game->so_texture.height);
    if (game->so_texture.img)
    {
        game->so_texture.addr = mlx_get_data_addr(game->so_texture.img,
            &game->so_texture.bits_per_pixel,
            &game->so_texture.line_length,
            &game->so_texture.endian);
    }
            
    // Load West texture
    game->we_texture.img = mlx_xpm_file_to_image(game->mlx, config->we_tex,
                                                   &game->we_texture.width,
                                                   &game->we_texture.height);
    if (game->we_texture.img)
        game->we_texture.addr = mlx_get_data_addr(game->we_texture.img,
                                                   &game->we_texture.bits_per_pixel,
                                                   &game->we_texture.line_length,
                                                   &game->we_texture.endian);

    // Load East texture
    game->ea_texture.img = mlx_xpm_file_to_image(game->mlx, config->ea_tex,
                                                   &game->ea_texture.width,
                                                   &game->ea_texture.height);
    if (game->ea_texture.img)
        game->ea_texture.addr = mlx_get_data_addr(game->ea_texture.img,
                                                   &game->ea_texture.bits_per_pixel,
                                                   &game->ea_texture.line_length,
                                                   &game->ea_texture.endian);
}

void creat_window(t_game *game, t_config *config)
{
    game->win_w = 1200;
    game->win_h = (config->map_h * TILE > 600) ? config->map_h * TILE : 600;
    game->config = config;

    game->mlx = mlx_init();
    if (!game->mlx)
        exit(EXIT_FAILURE);
    
    load_textures(game, config);
    printf("%s\n", config->no_tex);
    if (!game->no_texture.img)
        printf("Failed to load North texture: %s\n", config->no_tex);
    if (!game->so_texture.img)
        printf("Failed to load South texture: %s\n", config->so_tex);
    if (!game->we_texture.img)
        printf("Failed to load West texture: %s\n", config->we_tex);
    if (!game->ea_texture.img)
        printf("Failed to load East texture: %s\n", config->ea_tex);

    game->window = mlx_new_window(
        game->mlx,
        game->win_w,
        game->win_h,
        "CUB3D"
    );
    if (!game->window)
        exit(EXIT_FAILURE);
    
    init_player_from_config(game, config);
    game->mouse.is_pressed = 0;
    game->mouse.prev_x = game->win_w / 2;
    game->mouse.prev_y = game->win_h / 2;
  
    mlx_mouse_move(game->mlx, game->window, game->win_w / 2, game->win_h / 2);
    mlx_hook(game->window, 4, 1L << 2, mouse_press, game);
    mlx_hook(game->window, 5, 1L << 3, mouse_release, game);
    mlx_hook(game->window, 6, 1L << 6, mouse_move, game);
    mlx_hook(game->window, 2, 1L << 0, key_press, game);
    mlx_hook(game->window, 17, 1L << 17, close_window, game);

    draw_mini_and_rays(game, config);
    mlx_loop(game->mlx);
}