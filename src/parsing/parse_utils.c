/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:23:56 by iel-asef          #+#    #+#             */
/*   Updated: 2025/11/06 03:22:02 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"
#include <stdlib.h>
#include <string.h>

/* safe strdup fallback using malloc/memcpy (use ft_strdup if available) */
static char *str_dup_safe(const char *s)
{
    size_t n;
    char *d;

    if (!s)
        return (NULL);
    n = strlen(s);
    d = malloc(n + 1);
    if (!d)
        return (NULL);
    memcpy(d, s, n + 1);
    return (d);
}

/* append a line to a NULL-terminated array of strings, returns new array or NULL */
char	**add_line_to_array(char **arr, const char *line)
{
    char	**new_arr;
    int		count;
    int		i;

    if (!line)
        return (arr);
    if (!arr)
    {
        new_arr = malloc(sizeof(char *) * 2);
        if (!new_arr)
            return (NULL);
        new_arr[0] = str_dup_safe(line);
        if (!new_arr[0]) { free(new_arr); return (NULL); }
        new_arr[1] = NULL;
        return (new_arr);
    }
    count = 0;
    while (arr[count])
        count++;
    new_arr = malloc(sizeof(char *) * (count + 2));
    if (!new_arr)
        return (NULL);
    i = 0;
    while (i < count)
    {
        new_arr[i] = arr[i];
        i++;
    }
    new_arr[i++] = str_dup_safe(line);
    if (!new_arr[i-1])
    {
        free(new_arr);
        return (NULL);
    }
    new_arr[i] = NULL;
    free(arr);
    return (new_arr);
}

char	*ft_strdup_safe(const char *s)
{
	char	*d;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	d = malloc(len + 1);
	if (!d)
		return (NULL);
	memcpy(d, s, len);
	d[len] = '\0';
	return (d);
}


char	**duplicate_map(char **map)
{
	char	**dup;
	int		i;

	if (!map)
		return (NULL);
	i = 0;
	while (map[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (map[i])
	{
		dup[i] = ft_strdup_safe(map[i]);
		if (!dup[i])
		{
			while (i > 0)
			{
				i--;
				free(dup[i]);
			}
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}


int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int is_player_char(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/* free a NULL-terminated array of strings safely */
void	free_split_safe(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}