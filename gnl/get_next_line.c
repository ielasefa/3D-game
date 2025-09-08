/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-asef <iel-asef@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:17:27 by iel-asef          #+#    #+#             */
/*   Updated: 2024/12/02 18:12:44 by iel-asef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_read(int fd, char *storage)
{
	char		*buffer;
	ssize_t		read_bytes;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	read_bytes = 1;
	while (read_bytes > 0 && (storage == NULL || !ft_strchr(storage, '\n')))
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
			return (free(buffer), free(storage), storage = NULL, NULL);
		if (read_bytes == 0)
			break ;
		buffer[read_bytes] = '\0';
		temp = ft_strjoin(storage, buffer);
		if (!temp)
			return (free(buffer), free(storage), (NULL));
		free(storage);
		storage = temp;
	}
	return (free(buffer), (storage));
}

char	*ft_line(char *storage)
{
	char	*line;
	int		i;
	int		j;

	if (!storage)
		return (NULL);
	i = 0;
	j = 0;
	while (storage[i] && storage[i] != '\n')
		i++;
	if (storage[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
	{
		free(storage);
		return (NULL);
	}
	while (j < i)
	{
		line[j] = storage[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

char	*clean_storage(char *storage)
{
	char	*cpy;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!storage)
		return (free(storage), NULL);
	while (storage[i] && storage[i] != '\n')
		i++;
	if (!storage[i])
		return (free(storage), NULL);
	if (storage[i++] == '\n')
		if (!storage[i])
			return (free(storage), NULL);
	cpy = malloc(sizeof(char) * (ft_strlen(storage) - i + 1));
	if (!cpy)
		return (free(storage), NULL);
	while (storage[i])
		cpy[j++] = storage[i++];
	cpy[j] = '\0';
	free(storage);
	return (cpy);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;

	storage = ft_read(fd, storage);
	if (!storage)
		return (free(storage), storage = NULL, NULL);
	line = ft_line(storage);
	if (!line)
		return (free(storage), storage = NULL, NULL);
	storage = clean_storage(storage);
	return (line);
}
