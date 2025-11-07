#include "../../includes/cub.h"
#include <stdlib.h>
#include <string.h>

void doors_init(t_doors *doors)
{
    if (!doors)
        return ;
    doors->data = NULL;
    doors->count = 0;
    doors->capacity = 0;
}

void doors_free(t_doors *doors)
{
    if (!doors)
        return ;
    if (doors->data)
        free(doors->data);
    doors->data = NULL;
    doors->count = 0;
    doors->capacity = 0;
}

static int doors_grow(t_doors *doors)
{
    int		newcap;
    t_door	*newdata;
    int		i;

    if (!doors)
        return (0);
    newcap = (doors->capacity == 0) ? 8 : doors->capacity * 2;
    newdata = (t_door *)malloc(newcap * sizeof(t_door));
    if (!newdata)
        return (0);
    i = 0;
    if (doors->data)
    {
        while (i < doors->count)
        {
            newdata[i] = doors->data[i];
            i++;
        }
        free(doors->data);
    }
    doors->data = newdata;
    doors->capacity = newcap;
    return (1);
}

void doors_build(t_doors *doors, char **map, int h, int w)
{
    int	y;
    int	x;

    if (!doors || !map)
        return ;
    y = 0;
    while (y < h && map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == 'D')
            {
                if (doors->count >= doors->capacity)
                    if (!doors_grow(doors))
                        return ;
                doors->data[doors->count].x = x;
                doors->data[doors->count].y = y;
                doors->data[doors->count].open = 0;  /* استعمل open بدل is_open */
                doors->count++;
            }
            x++;
        }
        y++;
    }
}

t_door *find_door(t_doors *doors, int x, int y)
{
    int	i;

    if (!doors || !doors->data)
        return (NULL);
    i = 0;
    while (i < doors->count)
    {
        if (doors->data[i].x == x && doors->data[i].y == y)
            return (&doors->data[i]);
        i++;
    }
    return (NULL);
}

void toggle_door_in_front(t_config *cfg, t_doors *doors, double px, double py, double ang)
{
    int		tx;
    int		ty;
    t_door	*d;

    if (!cfg || !doors)
        return ;
    tx = (int)(px + cos(ang));
    ty = (int)(py + sin(ang));
    if (ty < 0 || tx < 0 || ty >= cfg->map_h || tx >= cfg->map_w)
        return ;
    if (!cfg->map[ty] || cfg->map[ty][tx] != 'D')
        return ;
    d = find_door(doors, tx, ty);
    if (d)
        d->open = !(d->open);  /* استعمل open بدل is_open */
}
