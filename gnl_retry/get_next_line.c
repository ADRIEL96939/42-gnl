/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoswald <aoswald@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:06:03 by aoswald           #+#    #+#             */
/*   Updated: 2024/09/12 22:17:02 by aoswald          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash = NULL;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	if (!stash)
	{
		stash = malloc(1);
		if (stash)
			stash[0] = '\0';
	}
	stash = fill_stash(&stash, fd);
	if (!stash || ft_strlen(stash) == 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(&stash);
	return (line);
}

ssize_t	check_input(char **stash, char *buffer, int fd)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read == -1)
	{
		free(*stash);
		*stash = NULL;
		free(buffer);
		return (-1);
	}
	return (bytes_read);
}

char	*fill_stash(char **stash, int fd)
{
	char	*buffer;
	ssize_t	bytes_read;
	char	*temp;

	buffer = malloc(BUFFER_SIZE + 1);
	while (!ft_strchr(*stash, '\n'))
	{
		bytes_read = check_input(stash, buffer, fd);
		if (bytes_read == -1)
			return (NULL);
		buffer[bytes_read] = '\0';
		if (bytes_read == 0)
			break ;
		temp = ft_strjoin(*stash, buffer);
		if (!temp)
		{
			free(buffer);
			return (NULL);
		}
		free(*stash);
		*stash = temp;
		temp = NULL;
	}
	free(buffer);
	return (*stash);
}

char	*extract_line(char **stash)
{
	int		i;
	int		j;
	char	*line;
	char	*new_stash;

	i = 0;
	j = 0;
	if (ft_strchr(*stash, '\n'))
	{
		line = malloc(ft_strlen_newline(*stash) + 2);
		while ((*stash)[i] != '\n')
			line[j++] = (*stash)[i++];
		line[j++] = '\n';
		line[j] = '\0';
		new_stash = ft_substr(*stash, i + 1, ft_strlen(*stash) - i - 1);
		if (!new_stash)
			return (NULL);
		free(*stash);
		*stash = new_stash;
		return (line);
	}
	else
		line = extract_null(stash);
	return (line);
}

char	*extract_null(char **stash)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	line = (char *)malloc(sizeof(char) * (ft_strlen(*stash) + 1));
	if (!line)
		return (NULL);
	while ((*stash)[i] != '\0')
		line[j++] = (*stash)[i++];
	line[j] = '\0';
	free(*stash);
	*stash = NULL;
	return (line);
}
