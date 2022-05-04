#include "minishell.h"

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

char	*get_next_line_hd(int fd)
{
	char	c;
	char	*line;

	line = NULL;
	c = 'J'; // le J C le S
	while (read(fd, &c, 1) > 0 && c != '\n')
		line = ft_append_char(line, c);
	if (!line && c == '\n')
		line = ft_strdup("");
	if (c != '\n')
		write(STDOUT_FILENO, "\n", 1);
	return (line);
}
