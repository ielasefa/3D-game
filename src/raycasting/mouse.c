#include "cub.h"

int	mouse_press(int button, int x, int y, t_game *game)
{
	(void)button;
	(void)x;
	(void)y;
	game->mouse.is_pressed = 1;
	mlx_mouse_move(game->mlx, game->window, game->win_w / 2, game->win_h / 2);
	game->mouse.prev_x = game->win_w / 2;
	game->mouse.prev_y = game->win_h / 2;
	mlx_mouse_hide(game->mlx, game->window);
	return (0);
}

int	mouse_release(int button, int x, int y, t_game *game)
{
	(void)button;
	(void)x;
	(void)y;
	game->mouse.is_pressed = 1;
	return (0);
}

int	mouse_move(int x, int y, t_game *game)
{
	int	center_x;
	int	center_y;
	int	delta_x;

	center_x = game->win_w / 2;
	center_y = game->win_h / 2;

	if (x == center_x && y == center_y)
		return (0);
	delta_x = x - center_x;
	if (delta_x > -3 && delta_x < 3)
		return (0);
	if (delta_x > 80)
		delta_x = 80;
	if (delta_x < -80)
		delta_x = -80;

	if (delta_x != 0)
	{
		game->player.angle += delta_x * SENSITIVITY;
		while (game->player.angle < 0)
			game->player.angle += 2 * M_PI;
		while (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
		mlx_mouse_move(game->mlx, game->window, center_x, center_y);
		draw_mini_and_rays(game, game->config);
	}

	return (0);
}
