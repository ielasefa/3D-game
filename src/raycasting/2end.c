
#include "../../includes/cub.h"

#include "../includes/cub.h"
#include <string.h>

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

int get_texture_pixel(t_texture *tex, int x, int y)
{
    if (!tex || !tex->addr || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return COLOR_WALL;
    
    char *pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return *(int *)pixel;
}

void draw_textured_vertical_strip(t_game *game, int x, int start, int end, 
                                   t_texture *tex, float wall_x, int slice_width)
{
    if (x < 0 || x >= game->win_w)
        return;
    
    if (!tex || !tex->addr)
    {
        draw_vertical_strip(game, x, start, end, COLOR_BLUE, slice_width);
        return;
    }
    
    int tex_x = (int)(wall_x * (float)tex->width);
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= tex->width) tex_x = tex->width - 1;
    
    int wall_height = end - start;
    
    for (int dx = 0; dx < slice_width && (x + dx) < game->win_w; dx++)
    {
        for (int y = start; y < end; y++)
        {
            if (y >= 0 && y < game->win_h)
            {
                int tex_y = ((y - start) * tex->height) / wall_height;
                if (tex_y < 0) tex_y = 0;
                if (tex_y >= tex->height) tex_y = tex->height - 1;
                
                int color = get_texture_pixel(tex, tex_x, tex_y);
                mlx_pixel_put(game->mlx, game->window, x + dx, y, color);
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
static float cast_ray_dda(t_game *game, t_config *cfg, float ray_angle, int *side, float *wall_x)
{
    if (!side || !wall_x)
        return 0;
    // float pos_x = TILE;
    // float pos_y =  TILE;
    float pos_x = game->player.x / TILE;
    float pos_y = game->player.y / TILE;
    float dir_x = cosf(ray_angle);
    float dir_y = sinf(ray_angle);

    int map_x = (int)pos_x;
    int map_y = (int)pos_y;

    float delta_dist_x = (dir_x == 0) ? 1e30 : fabsf(1 / dir_x);
    float delta_dist_y = (dir_y == 0) ? 1e30 : fabsf(1 / dir_y);
    float side_dist_x, side_dist_y;
    int step_x, step_y;

    if (dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (pos_x - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - pos_x) * delta_dist_x;
    }

    if (dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (pos_y - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - pos_y) * delta_dist_y;
    }

    int hit = 0;
    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            *side = 0;
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            *side = 1;
        }

        if (map_x < 0 || map_y < 0
            || map_y >= cfg->map_h
            || map_x >= (int)ft_strlen(cfg->map[map_y]))
            return 1e30; // return large distance if outside map

        if (cfg->map[map_y][map_x] == '1')
            hit = 1;
    }

    float perp_dist;
    if (*side == 0)
        perp_dist = (map_x - pos_x + (1 - step_x) / 2) / dir_x;
    else
        perp_dist = (map_y - pos_y + (1 - step_y) / 2) / dir_y;

    if (*side == 0)
        *wall_x = pos_y + perp_dist * dir_y;
    else
        *wall_x = pos_x + perp_dist * dir_x;
    *wall_x -= floorf(*wall_x);

    return perp_dist;
}

void draw_rays(t_game *game, t_config *config)
{
    float fov = FOV_DEG * M_PI / 180.0f;
    float ray_angle = game->player.angle - (fov / 2);
    float step = fov / (float)RAYS;
    float proj_dist = (game->win_w / 2) / tanf(fov / 2);

    for (int r = 0; r < RAYS; r++)
    {
        int side;
        float wall_x;
        float dist = cast_ray_dda(game, config, ray_angle, &side, &wall_x);

        if (dist == 1e30)
        {
            ray_angle += step;
            continue;
        }

        float line_height = (TILE * proj_dist) / dist;
        int draw_start = (game->win_h / 2) - (line_height / 2);
        int draw_end = (game->win_h / 2) + (line_height / 2);
        if (draw_start < 0) draw_start = 0;
        if (draw_end > game->win_h) draw_end = game->win_h;

        t_texture *tex;
        if (side == 0)
            tex = (cosf(ray_angle) > 0) ? &game->ea_texture : &game->we_texture;
        else
            tex = (sinf(ray_angle) > 0) ? &game->so_texture : &game->no_texture;

        int screen_x = (r * (game->win_w / RAYS));

        draw_vertical_strip(game, screen_x, 0, draw_start, COLOR_CEILING, (game->win_w / RAYS));
        draw_textured_vertical_strip(game, screen_x, draw_start, draw_end, tex, wall_x, (game->win_w / RAYS));
        draw_vertical_strip(game, screen_x, draw_end, game->win_h, COLOR_FLOOR, (game->win_w / RAYS));

        ray_angle += step;
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
        int wall_side = 0; // 0=NS, 1=EW

        while (!hit && dist < 2000.0f)
        {
            float prev_x = ray_x;
            
            ray_x += cosf(ray_angle) * STEP;
            ray_y += sinf(ray_angle) * STEP;
            dist += STEP;
            
            int map_x = (int)(ray_x / TILE);
            int map_y = (int)(ray_y / TILE);
            
            if (map_at(config, map_x, map_y) == '1')
            {
                hit = 1;
                // Determine which side was hit
                int prev_map_x = (int)(prev_x / TILE);
                // int prev_map_y = (int)(prev_y / TILE);
                
                if (prev_map_x != map_x)
                    wall_side = 1; // East-West wall
                else
                    wall_side = 0; // North-South wall
                break;
            }
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

            float wall_x;
            if (wall_side == 1) // EW wall - use Y coordinate
                wall_x = ray_y / TILE - floorf(ray_y / TILE);
            else // NS wall - use X coordinate
                wall_x = ray_x / TILE - floorf(ray_x / TILE);

            t_texture *wall_tex = NULL;
            if (wall_side == 0) // NS wall
            {
                if (cosf(ray_angle) > 0)
                    wall_tex = &game->ea_texture; // Facing East
                else
                    wall_tex = &game->we_texture; // Facing West
            }
            else // EW wall
            {
                if (sinf(ray_angle) > 0)
                    wall_tex = &game->so_texture; // Facing South
                else
                    wall_tex = &game->no_texture; // Facing North
            }

            draw_textured_vertical_strip(game, screen_x, start, end, wall_tex, wall_x, slice_width);

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
    cast_ray_dda(game, config, 0.0f, NULL, NULL); // test call to avoid unused function warning
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
        game->so_texture.addr = mlx_get_data_addr(game->so_texture.img,
                                                   &game->so_texture.bits_per_pixel,
                                                   &game->so_texture.line_length,
                                                   &game->so_texture.endian);

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

void	creat_window(t_game *game, t_config *config)
{
	game->win_w = 1200;
	game->win_h = (config->map_h * TILE > 600) ? config->map_h * TILE : 600;
	game->config = config;

	game->mlx = mlx_init();
	if (!game->mlx)
		exit(EXIT_FAILURE);
    load_textures(game, config);
    if (!game->ea_texture.img)
           printf("Failed to load texture: %s\n", config->ea_tex);
    else {
        printf("Texture loaded: %s (%dx%d)\n",
               config->ea_tex,
               game->ea_texture.width, game->ea_texture.height);
    }

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