#include "../../includes/cub.h"

static int is_valid_char(char c)
{
    return (c == '0' || c == '1' || c == 'N' || c == 'S' || 
            c == 'E' || c == 'W' || c == ' ');
}

static int is_surrounded_by_walls(char **map, int i, int j)
{
    // Check all 8 surrounding positions
    if (i > 0 && j > 0 && map[i][j] != '1')
    {
        if (map[i-1][j] == ' ' || map[i+1][j] == ' ' || 
            map[i][j-1] == ' ' || map[i][j+1] == ' ')
            return (0);
    }
    return (1);
}

int validate_map(char **map)
{
    int i = 0;
    int j;
    int player = 0;
    
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (!is_valid_char(map[i][j]))
                return (0);
            if (map[i][j] == 'N' || map[i][j] == 'S' || 
                map[i][j] == 'E' || map[i][j] == 'W')
            {
                player++;
                if (!is_surrounded_by_walls(map, i, j))
                    return (0);
            }
            j++;
        }
        i++;
    }
    return (player == 1);
}