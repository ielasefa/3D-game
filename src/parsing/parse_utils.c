/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:23:56 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/11 15:47:35 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

char	**add_line_to_array(char **array, char *line)
{
    char	**new_array;
    int		len;
    int		i;

    len = 0;
    if (array)
        while (array[len])
            len++;
    new_array = malloc(sizeof(char *) * (len + 2));
    if (!new_array)
        return (NULL);
    i = 0;
    while (i < len)
    {
        new_array[i] = array[i];
        i++;
    }
    new_array[i] = ft_strdup(line);
    new_array[i + 1] = NULL;
    if (array)
        free(array);
    return (new_array);
}