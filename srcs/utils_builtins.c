/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:36:03 by rahmed            #+#    #+#             */
/*   Updated: 2022/02/13 15:10:52 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_env_name_len(char *str, int getequal)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!getequal)
		{
			if (str[i] == '=')
			{
				ft_putendl_fd("unset: invalid parameter name", 2);
				return (0);
			}
			if (str[i + 1] == '\0')
				return (++i);
		}
		else if ((i >= 2) && (str[0] != '='))
		{
			if (str[i] == '=')
				return (++i);
		}
		i++;
	}
	return (0);
}

int	replace_existing_env(char *args, t_list *env)
{
	int		len;
	t_list	*tmp_env;
	char	*tmp;

	tmp_env = env;
	len = get_env_name_len(args, TRUE);
	if (!len)
		return (1);
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->content, args, len) == 0)
		{
			free(tmp_env->content);
			tmp = ft_strjoin(args, "\n");
			tmp_env->content = ft_strdup(tmp);
			free(tmp);
			return (1);
		}
		tmp_env = tmp_env->next;
	}
	return (0);
}

char	*ft_get_path(char *args)
{
	char	*tmp;
	char	*path;

	if ((args[0] == '~') || (args[0] == '/'))
		path = ft_get_special_path(args);
	else if (args[0] == '$')
		path = ft_get_env_path(args);
	else
	{
		path = getcwd(NULL, 0);
		tmp = ft_strjoin(path, "/");
		free(path);
		path = ft_strjoin(tmp, ft_del_at_whitespace(args));
		free(tmp);
	}
	return (path);
}

char	*ft_get_special_path(char *args)
{
	int		i_args;
	char	*path;
	char	*tmp;

	i_args = 0;
	path = NULL;
	if (args[i_args] == '~')
	{
		if (args[++i_args] == '/')
			i_args++;
		tmp = ft_strjoin(getenv("HOME"), "/");
		path = ft_strjoin(tmp, ft_del_at_whitespace(&args[i_args]));
		free(tmp);
	}
	else if (args[i_args] == '/')
		path = ft_strdup(ft_del_at_whitespace(&args[i_args]));
	return (path);
}

void	freed_chdir(char *path)
{
	if (chdir(path) == -1)
		perror("cd");
	if (path)
		free(path);
}
