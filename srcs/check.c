/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 03:34:18 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/26 18:12:19 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_unset_path(char **args, t_var *var)
{
	int	i;

	i = 0;
	while (args[++i])
	{
		if (ft_strequ_hd(args[i], "PATH"))
			break ;
	}
	if (args[i])
	{
		ft_free_split(var->path);
		var->path = NULL;
	}
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

void	wait_this_fk_process(t_var *var)
{
	int	i;
	int	status;
	int	x;

	x = 0;
	i = -1;
	status = 0;
	if (var->child)
	{
		while (++i < var->child)
		{
			waitpid(-1, &status, 0);
			if (WIFSIGNALED(status) && !x)
			{
				x = 1;
				if (WTERMSIG(status) == SIGQUIT)
					write(STDOUT_FILENO, "Quit: 3", 7);
				write(STDOUT_FILENO, "\n", 1);
				g.exit_status = 128 + WTERMSIG(status);
			}
		}
		if (!x)
			g.exit_status = status % 255;
	}
}
