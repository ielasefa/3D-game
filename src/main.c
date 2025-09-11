/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:50:05 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/10 23:59:02 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



int main(int ac ,char** av)
{
    t_maps *maps;
    
    if (ac != 2)
		return (ft_putendl_fd(USAGE_ERR, 2), 1);
	if (has_cub_extension(av[1]))
		return (1);

	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		return (perror("open"), 1);
}