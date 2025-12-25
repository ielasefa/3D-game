/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 03:01:57 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 04:18:19 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	parser_abort(t_config *cfg, int code)
{
	gnl_cleanup1(cfg);
	free_config(cfg);
	print_error(code);
	exit(1);
}

void	parser_abort_with_path(t_config *cfg, int code, char *path)
{
	gnl_cleanup1(cfg);
	free_config(cfg);
	print_error_path(code, path);
	free(path);
	exit(1);
}

void	free_array(char **arr, int size)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < size)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_split_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_config(t_config *cfg)
{
	if (!cfg)
		return ;
	free(cfg->no_tex);
	free(cfg->so_tex);
	free(cfg->we_tex);
	free(cfg->ea_tex);
	free(cfg->door_tex);
	free_split_array(cfg->original_map);
	free_array(cfg->map, cfg->map_h);
}
