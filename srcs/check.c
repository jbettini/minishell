/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 03:34:18 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 09:20:45 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int		check_the_build_for_env(char *args)
{
	if (ft_strequ_hd(args, "unset"))
		ft_putstr("env: export: No such file or directory\n");
	else if (ft_strequ_hd(args, "export"))
		ft_putstr("env: export: No such file or directory\n");
	else if (ft_strequ_hd(args, "exit"))
		ft_putstr("env: exit: No such file or directory\n");
	else
		return (0);
	return (1);
}

int	check_unset_path(char **path, t_env *env)
{
	int	i;

	i = 0;
	while (path[i])
	{
		if (ft_strequ_hd(path[i], "PATH"))
			break ;
		i++;
	}
	if (path[i])
	{
		ft_free_split(env->path);
		env->path = NULL;
	}
	return (0);
}

int	ft_isbuild(char *args)
{
	if (ft_strequ_hd(args, "exit"))
		return (1);
	else if (ft_strequ_hd(args, "env"))
		return (1);
	else if (ft_strequ_hd(args, "unset"))
		return (1);
	else if (ft_strequ_hd(args, "export"))
		return (1);
	else if (ft_strequ_hd(args, "pwd"))
		return (1);
	else if (ft_strequ_hd(args, "cd"))
		return (1);
	else if (ft_strequ_hd(args, "echo"))
		return (1);
	return (0);
}