/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 18:11:01 by rahmed            #+#    #+#             */
/*   Updated: 2022/05/05 17:16:55 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_append_char(char *str, char c)
{
	char	*res;
	int		i;

	res = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!res)
		return (NULL);
	if (!str)
	{
		res[0] = c;
		res[1] = '\0';
		return (res);
	}
	i = -1;
	while (str[++i])
		res[i] = str[i];
	res[i] = c;
	res[++i] = '\0';
	free(str);
	return (res);
}

char	*ft_get_next_line(int fd)
{
	char	c;
	char	*line;

	line = NULL;
	if (read(fd, &c, 0) == -1)
		return (NULL);
	while (read(fd, &c, 1) > 0 && c != '\n')
		line = ft_append_char(line, c);
	if (c == '\n')
		line = ft_append_char(line, c);
	return (line);
}
