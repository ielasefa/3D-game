/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 02:39:50 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 03:09:55 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (line[i] == '\n' || line[i] == '\0');
}

int	is_space(int c)
{
	return (c == ' ' || c == '\t');
}

char	*trim_ws(const char *s)
{
	return (ft_strtrim(s, " \t\r\n"));
}

int	ends_with_xpm(const char *p)
{
	const char	*dot = ft_strrchr(p, '.');

	return (dot && ft_strcmp(dot, ".xpm") == 0);
}

void	scan_row_for_player(t_config *cfg, int i, int *count)
{
	int		j;
	int		len;
	char	c;

	len = (int)ft_strlen(cfg->map[i]);
	j = 0;
	while (j < len)
	{
		c = cfg->map[i][j];
		if (is_player_char(c))
		{
			cfg->player_x = j;
			cfg->player_y = i;
			cfg->player_dir = c;
			(*count)++;
		}
		j++;
	}
}
