/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:23:34 by rahmed            #+#    #+#             */
/*   Updated: 2022/02/14 22:26:36 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_env_path(char *args)
{
	int		i_args;
	char	*path;
	char	*tmp;

	i_args = 0;
	while (args[i_args] && args[i_args] != '/')
		i_args++;
	path = ft_calloc(i_args + 1, sizeof(char));
	ft_memcpy(path, &args[1], i_args - 1);
	tmp = ft_strjoin(getenv(path), "/");
	free(path);
	path = ft_strjoin(tmp, ft_del_at_whitespace(&args[i_args + 1]));
	free(tmp);
	return (path);
}

void	ft_check_unset(char **args, t_list **env)
{
	int	unset_key;

	unset_key = 1;
	while (args[unset_key] != NULL)
	{
		ft_unset(args[unset_key], env);
		unset_key++;
	}
}

void	ft_unset(char *args, t_list **env)
{
	int		len;
	t_list	*current;

	current = *env;
	len = get_env_name_len(args, FALSE);
	if (!len)
		return ;
	if (ft_strncmp(current->content, args, len) == 0)
	{
		ft_lstdel_front(env);
		return ;
	}
	while (current->next)
	{
		if (ft_strncmp(current->next->content, args, len) == 0)
		{
			ft_lstdel_next(current);
			return ;
		}
		current = current->next;
	}
}

void	ft_check_export(char **args, t_list **env)
{
	int	export_key;

	export_key = 1;
	while (args[export_key] != NULL)
	{
		if (!ft_isdigit(args[export_key][0]) &&
		ft_env_alnum_underscore(args[export_key]))
			ft_export(args[export_key], env);
		else
			print_error("export: not a valid identifier");
		export_key++;
	}
}

void	ft_export(char *args, t_list **env)
{
	int		exist;
	t_list	*env_cpy;

	env_cpy = *env;
	exist = replace_existing_env(args, env_cpy);
	if (!exist)
		ft_lstadd_back(&env_cpy, ft_lstnew(ft_strjoin(args, "\n")));
}

int	ft_env_alnum_underscore(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && (str[i] != '_'))
			return (0);
		i++;
	}
	return (1);
}