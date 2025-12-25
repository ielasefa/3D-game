/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 04:16:44 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 04:20:03 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

static int	handle_textures(t_config *cfg, const char *s)
{
	if (!ft_strncmp(s, "NO", 2) && is_space((unsigned char)s[2]))
		return (set_texture_or_die(cfg, &cfg->no_tex, skip_id_and_spaces(s, 2)),
			1);
	if (!ft_strncmp(s, "SO", 2) && is_space((unsigned char)s[2]))
		return (set_texture_or_die(cfg, &cfg->so_tex, skip_id_and_spaces(s, 2)),
			1);
	if (!ft_strncmp(s, "WE", 2) && is_space((unsigned char)s[2]))
		return (set_texture_or_die(cfg, &cfg->we_tex, skip_id_and_spaces(s, 2)),
			1);
	if (!ft_strncmp(s, "EA", 2) && is_space((unsigned char)s[2]))
		return (set_texture_or_die(cfg, &cfg->ea_tex, skip_id_and_spaces(s, 2)),
			1);
	if (!ft_strncmp(s, "DO", 2) && is_space((unsigned char)s[2]))
		return (set_texture_or_die(cfg, &cfg->door_tex, skip_id_and_spaces(s,
					2)), 1);
	return (0);
}

static int	handle_colors(t_config *cfg, const char *s)
{
	if (s[0] == 'F' && is_space((unsigned char)s[1]))
	{
		if (cfg->floor[0] != -1 || cfg->floor[1] != -1 || cfg->floor[2] != -1)
			parser_abort(cfg, ERR_INVALID_RGB);
		if (parse_rgb(cfg->floor, (char *)skip_id_and_spaces(s, 1)))
			parser_abort(cfg, ERR_INVALID_RGB);
		return (1);
	}
	if (s[0] == 'C' && is_space((unsigned char)s[1]))
	{
		if (cfg->ceil[0] != -1 || cfg->ceil[1] != -1 || cfg->ceil[2] != -1)
			parser_abort(cfg, ERR_INVALID_RGB);
		if (parse_rgb(cfg->ceil, (char *)skip_id_and_spaces(s, 1)))
			parser_abort(cfg, ERR_INVALID_RGB);
		return (1);
	}
	return (0);
}

void	parse_identifier(t_config *cfg, char *line)
{
	const char	*s = line;

	if (handle_textures(cfg, s))
		return ;
	if (handle_colors(cfg, s))
		return ;
	if (is_map_line(line))
	{
		cfg->map = add_line_to_array(cfg->map, line);
		cfg->map_h++;
		if ((int)ft_strlen(line) > cfg->map_w)
			cfg->map_w = ft_strlen(line);
	}
	else if (line[0] != '\0')
		parser_abort(cfg, ERR_UNKNOWN);
}

int	has_double_comma(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == ',' && str[i + 1] == ',')
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && !ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
