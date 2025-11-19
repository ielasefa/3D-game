/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitlen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:42:00 by iel-asef          #+#    #+#             */
/*   Updated: 2025/09/11 21:42:08 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_splitlen(char **split)
{
	int	len;

	if (!split)
		return (0);
	len = 0;
	while (split[len])
		len++;
	return (len);
}
