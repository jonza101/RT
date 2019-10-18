/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkimberl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/12 13:33:54 by jkimberl          #+#    #+#             */
/*   Updated: 2019/10/01 12:25:23 by jkimberl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		check_result(const int fd, char **line, char **result)
{
	char		*source;
	size_t		i;

	i = 0;
	while (result[fd][i] != '\n' && result[fd][i] != '\0')
		i++;
	if (result[fd][i] == '\n')
	{
		source = result[fd];
		*line = ft_strsub(result[fd], 0, i);
		source = ft_strdup(result[fd] + i + 1);
		free(result[fd]);
		result[fd] = source;
	}
	else if (result[fd][i] == '\0')
	{
		*line = ft_strdup(result[fd]);
		ft_strdel(&result[fd]);
	}
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static char	*result[MAX_FD + 1];
	char		buff[BUFF_SIZE + 1];
	int			ret;
	char		*source;

	if (line == NULL || fd < 0 || fd > MAX_FD)
		return (-1);
	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if (result[fd] == NULL)
			result[fd] = ft_strnew(1);
		if (!(source = ft_strjoin(result[fd], buff)))
			return (-1);
		free(result[fd]);
		result[fd] = source;
		if (ft_strchr(buff, '\n'))
			break ;
	}
	if (ret < 0)
		return (-1);
	if (ret == 0 && (result[fd] == NULL || result[fd][0] == '\0'))
		return (0);
	return (check_result(fd, line, result));
}
