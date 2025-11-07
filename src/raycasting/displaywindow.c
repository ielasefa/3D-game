#include "../../includes/cub.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/* ---------- HELPERS ---------- */

static int rgb_to_int(int rgb[3])
{
    return (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
}

static void put_pixel_to_image(t_game *game, int x, int y, int color)
{
    if (x < 0 || x >= game->win_w || y < 0 || y >= game->win_h)
        return;
    char *dst = game->img_addr + (y * game->img_line_len + x * (game->img_bpp / 8));
    *(unsigned int *)dst = color;
}

/* ---------- MINIMAP ---------- */

void draw_pixel(t_game *game, int i, int j, int color)
{
    for (int x = 0; x < TILE; x++)
    {
        for (int y = 0; y < TILE; y++)
        {
            put_pixel_to_image(game,
                i * TILE + x + MINIMAP_OX,
                j * TILE + y + MINIMAP_OY,
                color);
        }
    }
}

void draw_mini_map(t_game *game, t_config *config)
{
    for (int i = 0; config->map[i]; i++)
    {
        for (int j = 0; config->map[i][j]; j++)
        {
            if (config->map[i][j] == '1')
                draw_pixel(game, j, i, COLOR_WHITE);
            else if (config->map[i][j] == 'D')
                draw_pixel(game, j, i, 0xFF6600); // door color
            else
                draw_pixel(game, j, i, COLOR_BLUE);
        }
    }
    int px = (int)(game->player.x / TILE);
    int py = (int)(game->player.y / TILE);
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            put_pixel_to_image(game,
                MINIMAP_OX + px * TILE + dx,
                MINIMAP_OY + py * TILE + dy,
                COLOR_PINK);
}

/* ---------- RAYCAST HELPERS ---------- */

int map_at(t_config *cfg, int mx, int my)
{
    if (my < 0 || mx < 0 || my >= cfg->map_h || mx >= cfg->map_w)
        return '1';
    if (!cfg->map[my] || (int)ft_strlen(cfg->map[my]) <= mx)
        return '1';
    return cfg->map[my][mx];
}

void init_player_from_config(t_game *game, t_config *config)
{
    game->player.x = config->player_x * TILE + TILE / 2.0;
    game->player.y = config->player_y * TILE + TILE / 2.0;
    if (config->player_dir == 'N')
        game->player.angle = -M_PI / 2;
    else if (config->player_dir == 'S')
        game->player.angle = M_PI / 2;
    else if (config->player_dir == 'W')
        game->player.angle = M_PI;
    else
        game->player.angle = 0;
}

/* ---------- RAYCAST CORE ---------- */

void init_raycasting(t_game *game, t_dda *dda, int x)
{
    dda->pos_x = game->player.x / TILE;
    dda->pos_y = game->player.y / TILE;
    float fov_rad = FOV_DEG * M_PI / 180.0f;
    float plane_len = tanf(fov_rad / 2.0f);
    dda->dir_x = cosf(game->player.angle);
    dda->dir_y = sinf(game->player.angle);
    dda->plane_x = -dda->dir_y * plane_len;
    dda->plane_y = dda->dir_x * plane_len;
    dda->cam_x = 2 * x / (float)game->win_w - 1;
    dda->ray_dir_x = dda->dir_x + dda->plane_x * dda->cam_x;
    dda->ray_dir_y = dda->dir_y + dda->plane_y * dda->cam_x;
    dda->map_x = (int)dda->pos_x;
    dda->map_y = (int)dda->pos_y;
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
        if (cell == '1' || cell == 'D')
            break;
    }
}

/* ---------- TEXTURED DRAW ---------- */

static int get_texture_pixel(t_texture *tex, int x, int y)
{
    if (!tex || !tex->addr)
        return COLOR_WALL;
    x = (x < 0) ? 0 : (x >= tex->width ? tex->width - 1 : x);
    y = (y < 0) ? 0 : (y >= tex->height ? tex->height - 1 : y);
    return *(int *)(tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8)));
}

void draw_column(t_game *game, t_config *config, t_dda *dda, int x)
{
    double perp = (dda->wall_side == 0)
                    ? (dda->side_dist_x - dda->delta_dist_x)
                    : (dda->side_dist_y - dda->delta_dist_y);
    if (perp < 0.01f)
        perp = 0.01f;

    int wall_h = (int)(game->win_h / perp);
    int start = -wall_h / 2 + game->win_h / 2;
    if (start < 0) start = 0;
    int end = wall_h / 2 + game->win_h / 2;
    if (end >= game->win_h) end = game->win_h - 1;

    t_texture *tex;
    char cell = map_at(config, dda->map_x, dda->map_y);
    if (cell == 'D' && game->door_texture.img)
        tex = &game->door_texture;
    else
    {
        if (dda->wall_side == 0 && dda->ray_dir_x > 0)
            tex = &game->ea_texture;
        else if (dda->wall_side == 0)
            tex = &game->we_texture;
        else if (dda->wall_side == 1 && dda->ray_dir_y > 0)
            tex = &game->so_texture;
        else
            tex = &game->no_texture;
    }

    double wall_x = (dda->wall_side == 0)
                    ? (dda->pos_y + perp * dda->ray_dir_y)
                    : (dda->pos_x + perp * dda->ray_dir_x);
    wall_x -= floor(wall_x);
    int tex_x = (int)(wall_x * tex->width);

    for (int y = 0; y < game->win_h; y++)
    {
        if (y < start)
            put_pixel_to_image(game, x, y, rgb_to_int(config->ceil));
        else if (y > end)
            put_pixel_to_image(game, x, y, rgb_to_int(config->floor));
        else
        {
            int tex_y = (int)((y - start) * tex->height / (float)(end - start));
            int color = get_texture_pixel(tex, tex_x, tex_y);
            put_pixel_to_image(game, x, y, color);
        }
    }
}

void draw_rays(t_game *game, t_config *config)
{
    for (int x = 0; x < game->win_w; x++)
    {
        t_dda dda = {0};
        init_raycasting(game, &dda, x);
        get_delta_dist(&dda);
        get_step_and_sidedist(&dda);
        digital_differential_analyser(config, &dda);
        draw_column(game, config, &dda, x);
    }
}

/* ---------- WINDOW ---------- */

int close_window(t_game *game)
{
    mlx_clear_window(game->mlx, game->window);
    mlx_destroy_window(game->mlx, game->window);
    exit(0);
    return 0;
}

void draw_mini_and_rays(t_game *game, t_config *config)
{
    draw_rays(game, config);
    draw_mini_map(game, config);
    mlx_put_image_to_window(game->mlx, game->window, game->img, 0, 0);
}

/* ---------- LOAD TEXTURES ---------- */

void load_textures(t_game *game, t_config *config)
{
    t_texture *t[5] = { &game->no_texture, &game->so_texture,
                        &game->we_texture, &game->ea_texture, &game->door_texture };
    char *paths[5] = { config->no_tex, config->so_tex,
                       config->we_tex, config->ea_tex, config->door_tex };

    for (int i = 0; i < 5; i++)
    {
        if (!paths[i]) continue;
        t[i]->img = mlx_xpm_file_to_image(game->mlx, paths[i],
                                          &t[i]->width, &t[i]->height);
        if (t[i]->img)
        {
            t[i]->addr = mlx_get_data_addr(t[i]->img,
                            &t[i]->bits_per_pixel, &t[i]->line_length, &t[i]->endian);
        }
        else
        {
            fprintf(stderr, "⚠️ Failed to load texture: %s\n", paths[i]);
        }
    }
}

/* ---------- CREATE WINDOW ---------- */

void creat_window(t_game *game, t_config *config)
{
    game->win_w = 1200;
    game->win_h = fmax(600, config->map_h * TILE);

    game->mlx = mlx_init();
    if (!game->mlx)
        exit(EXIT_FAILURE);

    game->img = mlx_new_image(game->mlx, game->win_w, game->win_h);
    game->img_addr = mlx_get_data_addr(game->img,
                                       &game->img_bpp,
                                       &game->img_line_len,
                                       &game->img_endian);
    load_textures(game, config);

    game->window = mlx_new_window(game->mlx, game->win_w, game->win_h, "CUB3D");
    if (!game->window)
        exit(EXIT_FAILURE);

    init_player_from_config(game, config);
    game->mouse.is_pressed = 0;
    game->mouse.prev_x = game->win_w / 2;
    game->mouse.prev_y = game->win_h / 2;

    mlx_hook(game->window, 2, 1L << 0, key_press, game);
    mlx_hook(game->window, 4, 1L << 2, mouse_press, game);
    mlx_hook(game->window, 5, 1L << 3, mouse_release, game);
    mlx_hook(game->window, 6, 1L << 6, mouse_move, game);
    mlx_hook(game->window, 17, 1L << 17, close_window, game);

    draw_mini_and_rays(game, config);
    mlx_loop(game->mlx);
}

void free_config(t_config *config)
{
    if (!config)
        return;

    if (config->map)
    {
        free_split_safe(config->map);
        config->map = NULL;
    }
    if (config->original_map)
    {
        free_split_safe(config->original_map);
        config->original_map = NULL;
    }

    free(config->no_tex);
    free(config->so_tex);
    free(config->we_tex);
    free(config->ea_tex);
    free(config->door_tex);

    config->map_w = 0;
    config->map_h = 0;
}
