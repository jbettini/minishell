/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 01:39:22 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/01 21:14:11 by jbettini         ###   ########.fr       */
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
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		dup2(fd[1], 1);
		close(fd[0]);
		exec_in_main(cmd, env, IN_PIPE);
	}
	else
	{
		env->child++;
		dup2(fd[0], 0);
		close(fd[1]);
		close(fd[0]);
	}
}
