/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 00:13:41 by ylahssin          #+#    #+#             */
/*   Updated: 2025/04/12 21:21:50 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_free(char **store)
{
	free(*store);
	*store = NULL;
	return (*store);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*result;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	result = malloc(nmemb * size);
	if (!result)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
		result[i++] = 0;
	return (result);
}

int	ft_read_from_file(int fd, char *buffer, char **store)
{
	int		read_byte;
	char	*tmp;

	read_byte = read(fd, buffer, BUFFER_SIZE);
	if (read_byte <= 0)
		return (read_byte);
	buffer[read_byte] = '\0';
	tmp = ft_strjoin(*store, buffer);
	if (!tmp)
		return (-1);
	ft_free(store);
	return (*store = tmp, read_byte);
}

char	*ft_extract_line(char **store)
{
	char	*new_pos;
	char	*tmp;
	char	*line;

	if (!*store || !**store)
		return (NULL);
	new_pos = ft_strchr(*store, '\n');
	if (new_pos)
	{
		line = ft_substr(*store, 0, new_pos - *store + 1);
		if (!line)
			return (NULL);
		tmp = ft_strdup(*store + (new_pos - *store + 1));
		if (!tmp)
			return (ft_free(&line));
		return (ft_free(store), *store = tmp, line);
	}
	if (*store || **store)
		return (line = ft_strdup(*store), ft_free(store), line);
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*store;
	char		*buffer;
	char		*line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= 2147483647 || read(fd, 0,
			0) == -1)
		return (ft_free(&store));
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (ft_free(&store));
	while (!ft_strchr(store, '\n'))
	{
		bytes_read = ft_read_from_file(fd, buffer, &store);
		if (bytes_read == -1)
			return (free(buffer), ft_free(&store));
		else if (bytes_read == 0)
			break ;
	}
	free(buffer);
	line = ft_extract_line(&store);
	if (!line || !*line)
		return (ft_free(&store));
	return (line);
}
