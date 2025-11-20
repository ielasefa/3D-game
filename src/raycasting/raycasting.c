
#include "../includes/cub.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

unsigned int timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned int)tv.tv_sec;
}

static int rgb_to_int(int rgb[3])
{
    return (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
}

static void put_pixel_to_image(t_game *game, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= game->win_w || y < 0 || y >= game->win_h)
        return;

    dst = game->img_addr + (y * game->img_line_len + x * (game->img_bpp / 8));
    *(unsigned int*)dst = color;
}

int map_at(t_config *cfg, int mx, int my)
{
    if (my < 0 || mx < 0 || my >= cfg->map_h || mx >= cfg->map_w)
        return '1';
    if (!cfg->map[my] || (int)ft_strlen(cfg->map[my]) <= mx)
        return '1';
    return cfg->map[my][mx];
}

int get_texture_pixel(t_texture *tex, int x, int y)
{
    if (!tex || !tex->addr || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return COLOR_WALL;

    char *pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return *(int *)pixel;
}

void load_hand_reload(t_game *game)
{
    char path[64];
    for (int i = 0; i < 12; i++)
    {
        snprintf(path, sizeof(path), "./textures/hand_reload/hand_%d.xpm", i);
        game->hand_texture[i].img = mlx_xpm_file_to_image(game->mlx,
            path, &game->hand_texture[i].width, &game->hand_texture[i].height);
        if (game->hand_texture[i].img)
            game->hand_texture[i].addr = mlx_get_data_addr(game->hand_texture[i].img,
                &game->hand_texture[i].bits_per_pixel,
                &game->hand_texture[i].line_length,
                &game->hand_texture[i].endian);
    }
    game->hands_anim_frame = 0;
}

static void draw_hands_pixel(t_game *game, int x, int y, t_hands_pixel *pixel)
{
    int color = get_texture_pixel(&game->hand_texture[game->hands_anim_frame], x, y);
    if ((color & 0x00FFFFFF) != 0xFF00FF && (color & 0x00FFFFFF) != 0x000000)
        put_pixel_to_image(game, pixel->screen_x + x, pixel->screen_y + y, color);
}

void draw_hands(t_game *game)
{
    t_hands_pixel pixel;
    t_texture *tex = &game->hand_texture[game->hands_anim_frame];
    
    if (!tex || !tex->addr) 
        return;
    
    pixel.screen_x = (game->win_w - tex->width) / 2;
    pixel.screen_y = game->win_h - tex->height + (int)(sin(game->hands_anim_frame * 0.2) * 4.0);
    
    for (int y = 0; y < tex->height; y++)
        for (int x = 0; x < tex->width; x++)
            draw_hands_pixel(game, x, y, &pixel);
}

void update_hand_animation(t_game *game)
{
    static int last_anim_time = 0;
    static int frame_counter = 0;
    int current_time = timestamp();
    
    if (game->hands_anim_frame >= 11 && current_time - last_anim_time <= 8)
        return;
    if (last_anim_time == 0)
    {
        last_anim_time = current_time;
        game->hands_anim_frame = 0;
        return;
    }
    if (current_time - last_anim_time >= 5)
    {
        game->hands_anim_frame = 0;
        frame_counter = 0;
        last_anim_time = current_time;
        return;
    }
    frame_counter++;
    if (frame_counter >= 9)
    {
        game->hands_anim_frame++;
        if (game->hands_anim_frame >= 12)
        {
            game->hands_anim_frame = 11;
        }
        frame_counter = 0;
    }
}

void draw_pixel(t_game *game, int i, int j, int color)
{
    for (int x = 0; x < TILE; x++)
        for (int y = 0; y < TILE; y++)
            put_pixel_to_image(game,
                              i * TILE + x + MINIMAP_OX,
                              j * TILE + y + MINIMAP_OY,
                              color);
}

void draw_mini_map_tiles(t_game *game, t_config *config)
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
}

void draw_mini_map(t_game *game, t_config *config)
{
    static int tiles_drawn = 0;
    
    if (!tiles_drawn || game->player.x != game->player.prev_x || 
        game->player.y != game->player.prev_y)
    {
        draw_mini_map_tiles(game, config);
        tiles_drawn = 1;
    }
    else
        draw_mini_map_tiles(game, config);
    int px = (int)roundf(game->player.x) + MINIMAP_OX;
    int py = (int)roundf(game->player.y) + MINIMAP_OY;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            put_pixel_to_image(game, px + dx, py + dy, COLOR_PINK);
}

void get_delta_dist(t_dda *dda)
{
    dda->delta_dist_x = (dda->ray_dir_x == 0) ? 1e30 : fabsf(1 / dda->ray_dir_x);
    dda->delta_dist_y = (dda->ray_dir_y == 0) ? 1e30 : fabsf(1 / dda->ray_dir_y);
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
    while (1)
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
        char cell = map_at(config, dda->map_x, dda->map_y);
        if (cell == '1' || cell == 'D') break;
    }
}

void draw_vertical_strip(t_game *game, int x, int start, int end, int color, int width)
{
    for (int dx = 0; dx < width && x + dx < game->win_w; dx++)
        for (int y = start; y < end; y++)
            if (y >= 0 && y < game->win_h)
                put_pixel_to_image(game, x + dx, y, color);
}

void draw_textured_vertical_strip(t_game *game, int x, int start, int end,
                                  t_texture *tex, int tex_x, int width)
{
    if (!tex || !tex->addr)
    {
        draw_vertical_strip(game, x, start, end, COLOR_WALL, width);
        return;
    }

    int height = end - start;
    float step = (float)tex->height / height;
    float tex_pos = 0;

    for (int dx = 0; dx < width && x + dx < game->win_w; dx++)
    {
        float current = tex_pos;
        for (int y = start; y < end; y++)
        {
            int tex_y = (int)current % tex->height;
            int color = get_texture_pixel(tex, tex_x, tex_y);
            put_pixel_to_image(game, x + dx, y, color);
            current += step;
        }
    }
}

void draw_column(t_game *game, t_config *cfg, t_dda *dda, int x)
{
    dda->perp_wall_dist = (dda->wall_side == 0)
        ? dda->side_dist_x - dda->delta_dist_x
        : dda->side_dist_y - dda->delta_dist_y;

    if (dda->perp_wall_dist < 0.1f) dda->perp_wall_dist = 0.1f;

    int line_height = (int)(game->win_h / dda->perp_wall_dist);
    int draw_start = -line_height / 2 + game->win_h / 2;
    int draw_end = line_height / 2 + game->win_h / 2;
    if (draw_start < 0) draw_start = 0;
    if (draw_end >= game->win_h) draw_end = game->win_h - 1;

    float wall_x = (dda->wall_side == 0)
        ? dda->pos_y + dda->perp_wall_dist * dda->ray_dir_y
        : dda->pos_x + dda->perp_wall_dist * dda->ray_dir_x;
    wall_x -= floorf(wall_x);

    t_texture *tex;
    if (dda->wall_side == 0)
    {
        if (dda->ray_dir_x > 0)
            tex = &game->ea_texture; // East wall
        else
            tex = &game->we_texture; // West wall
    }
    else
    {
        if (dda->ray_dir_y > 0)
            tex = &game->so_texture; // South wall
        else
            tex = &game->no_texture; // North wall
    }

    {
        char cell = map_at(cfg, dda->map_x, dda->map_y);
        if (cell == 'D' && game->door_texture.addr)
            tex = &game->door_texture;
    }

    int tex_x = (int)(wall_x * tex->width);
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= tex->width) tex_x = tex->width - 1;

    int ceil_col = rgb_to_int(cfg->ceil);
    int floor_col = rgb_to_int(cfg->floor);

    draw_vertical_strip(game, x, 0, draw_start, ceil_col, 1);
    draw_textured_vertical_strip(game, x, draw_start, draw_end, tex, tex_x, 1);
    draw_vertical_strip(game, x, draw_end, game->win_h, floor_col, 1);
}

void draw_rays(t_game *game, t_config *cfg)
{
    t_dda dda;
    for (int x = 0; x < game->win_w; x++)
    {
        init_raycasting(game, &dda, x);
        get_delta_dist(&dda);
        get_step_and_sidedist(&dda);
        digital_differential_analyser(cfg, &dda);
        draw_column(game, cfg, &dda, x);
    }
}

void update_display(t_game *game, t_config *config)
{
    mlx_put_image_to_window(game->mlx, game->window, game->img, 0, 0);
    draw_hands(game);
}

void draw_mini_and_rays(t_game *game, t_config *config)
{
    update_hand_animation(game);
    draw_hands(game);
    draw_mini_map(game, config);
    draw_rays(game, config);
    mlx_put_image_to_window(game->mlx, game->window, game->img, 0, 0);
}
int tick(t_game *game)
{
    update_hand_animation(game);
    draw_rays(game, game->config);
    draw_mini_map(game, game->config);
    game->player.prev_x = game->player.x;
    game->player.prev_y = game->player.y;
    draw_hands(game);
    mlx_put_image_to_window(game->mlx, game->window, game->img, 0, 0);
    return 0;
}

int close_window(t_game *game)
{
    if (game)
    {
        if (game->mlx)
        {
            if (game->img)
                mlx_destroy_image(game->mlx, game->img);
            if (game->no_texture.img)
                mlx_destroy_image(game->mlx, game->no_texture.img);
            if (game->so_texture.img)
                mlx_destroy_image(game->mlx, game->so_texture.img);
            if (game->we_texture.img)
                mlx_destroy_image(game->mlx, game->we_texture.img);
            if (game->ea_texture.img)
                mlx_destroy_image(game->mlx, game->ea_texture.img);
            if (game->door_texture.img)
                mlx_destroy_image(game->mlx, game->door_texture.img);

            for (int i = 0; i < 12; ++i)
            {
                if (game->hand_texture[i].img)
                    mlx_destroy_image(game->mlx, game->hand_texture[i].img);
            }

            if (game->window)
                mlx_destroy_window(game->mlx, game->window);
        }
        if (game->config)
            free_config(game->config);
    }
    /* delegate to central cleanup function */
    cleanup_and_exit(game);
    return (0);
}

void load_textures(t_game *game, t_config *config)
{
    game->no_texture.img = mlx_xpm_file_to_image(game->mlx, config->no_tex,
        &game->no_texture.width, &game->no_texture.height);
    if (game->no_texture.img)
        game->no_texture.addr = mlx_get_data_addr(game->no_texture.img,
            &game->no_texture.bits_per_pixel, &game->no_texture.line_length,
            &game->no_texture.endian);

    game->so_texture.img = mlx_xpm_file_to_image(game->mlx, config->so_tex,
        &game->so_texture.width, &game->so_texture.height);
    if (game->so_texture.img)
        game->so_texture.addr = mlx_get_data_addr(game->so_texture.img,
            &game->so_texture.bits_per_pixel, &game->so_texture.line_length,
            &game->so_texture.endian);

    game->we_texture.img = mlx_xpm_file_to_image(game->mlx, config->we_tex,
        &game->we_texture.width, &game->we_texture.height);
    if (game->we_texture.img)
        game->we_texture.addr = mlx_get_data_addr(game->we_texture.img,
            &game->we_texture.bits_per_pixel, &game->we_texture.line_length,
            &game->we_texture.endian);

    game->ea_texture.img = mlx_xpm_file_to_image(game->mlx, config->ea_tex,
        &game->ea_texture.width, &game->ea_texture.height);
    if (game->ea_texture.img)
        game->ea_texture.addr = mlx_get_data_addr(game->ea_texture.img,
            &game->ea_texture.bits_per_pixel, &game->ea_texture.line_length,
            &game->ea_texture.endian);

    if (config->door_tex)
    {
        game->door_texture.img = mlx_xpm_file_to_image(game->mlx, config->door_tex,
            &game->door_texture.width, &game->door_texture.height);
        if (game->door_texture.img)
            game->door_texture.addr = mlx_get_data_addr(game->door_texture.img,
                &game->door_texture.bits_per_pixel, &game->door_texture.line_length,
                &game->door_texture.endian);
        /* Informational: print whether door texture loaded (helpful for debugging) */
        if (game->door_texture.img)
            printf("[info] loaded door texture: %s\n", config->door_tex);
        else
            printf("[warn] failed to load door texture: %s\n", config->door_tex);
    }

    load_hand_reload(game);
}

void creat_window(t_game *game, t_config *config)
{
    game->win_w = 1200;
    game->win_h = (config->map_h * TILE > 600) ? config->map_h * TILE : 600;
    game->config = config;

    game->mlx = mlx_init();
    if (!game->mlx) exit(EXIT_FAILURE);

    game->img = mlx_new_image(game->mlx, game->win_w, game->win_h);
    game->img_addr = mlx_get_data_addr(game->img, &game->img_bpp,
                                       &game->img_line_len, &game->img_endian);

    load_textures(game, config);

    game->window = mlx_new_window(game->mlx, game->win_w, game->win_h, "CUBE YA ZEBU");
    if (!game->window) exit(EXIT_FAILURE);

    init_player_from_config(game, config);

    mlx_hook(game->window, 4, 1L<<2, mouse_press, game);
    mlx_hook(game->window, 5, 1L<<3, mouse_release, game);
    mlx_hook(game->window, 6, 1L<<6, mouse_move, game);
    mlx_hook(game->window, 2, 1L<<0, key_press, game);
    mlx_hook(game->window, 17, 1L<<17, close_window, game);
    game->last_frame_time = timestamp();
    mlx_loop_hook(game->mlx, tick, game);
    mlx_loop(game->mlx);
}