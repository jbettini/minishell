/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 01:39:22 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/16 07:45:55 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pipex(t_cmd *cmd, t_env *env)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (!pid)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		exec_in_main(cmd, env, 0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		dup2(fd[0], 0);
		close(fd[1]);
	}
}
