/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 04:17:56 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 04:23:13 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"


void	set_texture_or_die(t_config *cfg, char **dst, const char *raw)
{
	char	*path;

	if (*dst)
	{
		gnl_cleanup1(cfg);
		parser_abort(cfg, ERR_INVALID_PATH);
	}
	path = ft_strtrim(raw, " \t\r\n");
	validate_path_or_die(cfg, path, 1);
	*dst = path;
}

void	free_partial_dup(char **dup, int i)
{
	while (i > 0)
	{
		i--;
		free(dup[i]);
	}
	free(dup);
}
