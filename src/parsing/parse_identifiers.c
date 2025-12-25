/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 20:12:21 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 04:22:14 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

int	file_readable(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

void	validate_path_or_die(t_config *cfg, char *path, int check_ext)
{
	if (!path || *path == '\0')
	{
		gnl_cleanup1(cfg);
		free(path);
		parser_abort(cfg, ERR_INVALID_PATH);
	}
	if (check_ext && !ends_with_xpm(path))
	{
		gnl_cleanup1(cfg);
		free(path);
		parser_abort(cfg, ERR_INVALID_EXT);
	}
	if (!file_readable(path))
	{
		gnl_cleanup1(cfg);
		parser_abort_with_path(cfg, ERR_INVALID_PATH, path);
	}
}

const char	*skip_id_and_spaces(const char *s, int idlen)
{
	s += idlen;
	while (*s && is_space((unsigned char)*s))
		s++;
	return (s);
}

int	validate_rgb_component(const char *s, int *value, char **split)
{
	char	*trimmed;

	trimmed = ft_strtrim(s, " \t");
	if (!trimmed || !*trimmed || !is_valid_number(trimmed))
	{
		ft_free_split(split);
		free(trimmed);
		return (1);
	}
	*value = ft_atoi(trimmed);
	free(trimmed);
	if (*value < 0 || *value > 255)
	{
		ft_free_split(split);
		return (1);
	}
	return (0);
}

int	parse_rgb(int color[3], char *s)
{
	char	**split;
	int		i;

	if (!s)
		return (1);
	while (*s && (*s == ' ' || *s == '\t'))
		s++;
	if (*s == ',' || s[ft_strlen(s) - 1] == ',' || has_double_comma(s))
		return (1);
	split = ft_split(s, ',');
	if (!split || ft_splitlen(split) != 3)
		return (ft_free_split(split), 1);
	i = 0;
	while (i < 3)
	{
		if (validate_rgb_component(split[i], &color[i], split))
			return (1);
		i++;
	}
	ft_free_split(split);
	return (0);
}
