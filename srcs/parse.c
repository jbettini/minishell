/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:44 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 21:49:09 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*parse(char *line)
{
	t_list	*tokens;
	t_list	*cmds;

	tokens = get_tokens(line);
	if (!tokens)
		return (NULL);
	cmds = get_cmds(tokens);
	ft_lstclear(&tokens, &free_token);
	return (cmds);
}

char	*parse_cmd(char **path, char **cmd)
{
	char	*cmd_path;
	char	*tmp;
	size_t	i;

	if (!path)
		return (NULL);
	i = -1;
	tmp = ft_strjoin("/", cmd[0]);
	while (path[++i])
	{
		cmd_path = ft_strjoin(path[i], tmp);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free(tmp);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free(tmp);
	return (NULL);
}

void	print_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = -1;
	while (strs[i])
		printf("%s\n", strs[i++]);
}
