/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 03:01:57 by iel-asef          #+#    #+#             */
/*   Updated: 2025/12/25 03:03:55 by iel-asef         ###   ########.fr       */
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