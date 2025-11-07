#include "../../includes/cub.h"

static int	map_height(char **map)
{
	int	h = 0;
	while (map && map[h])
		h++;
	return (h);
}

static int	row_len(char **map, int y)
{
	if (!map || !map[y])
		return (0);
	return (ft_strlen(map[y]));
}

static int	player_inside_cell(double px, double py, int cx, int cy)
{
	double dx = px - (cx + 0.5);
	double dy = py - (cy + 0.5);
	return (sqrt(dx * dx + dy * dy) < 0.5);
}

int	was_door_before(int x, int y, char **orig)
{
	if (!orig || y < 0 || x < 0)
		return (0);
	if (y >= map_height(orig) || x >= row_len(orig, y))
		return (0);
	return (orig[y][x] == 'D');
}

void	toggle_door(char **map, char **original_map,
	double player_x, double player_y, double dir_x, double dir_y)
{
	int		tx = (int)floor(player_x + dir_x);
	int		ty = (int)floor(player_y + dir_y);
	double	dx = (tx + 0.5) - player_x;
	double	dy = (ty + 0.5) - player_y;
	double	dist = hypot(dx, dy);

	if (!map || dist > 1.5)
		return ;
	if (ty < 0 || tx < 0 || ty >= map_height(map)
		|| tx >= row_len(map, ty))
		return ;
	if (map[ty][tx] == 'D')
		map[ty][tx] = '0';
	else if (map[ty][tx] == '0' && was_door_before(tx, ty, original_map))
	{
		if (!player_inside_cell(player_x, player_y, tx, ty))
			map[ty][tx] = 'D';
	}
}
