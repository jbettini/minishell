/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 01:39:22 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/23 07:33:29 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_env_exec_in_pipe(t_cmd *cmd, t_env *env)
{
	char **tmp;

	tmp = &(cmd->args[0]);
	if (ft_strequ_hd(cmd->args[0], "env"))
	{
		tmp = remake_path(env, tmp);
		ft_free_split(cmd->args);
		cmd->args = tmp;
	}
}

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
		exec_in_main(cmd, env, IN_PIPE);
	}
	else
	{
		env->child++;
		dup2(fd[0], 0);
		close(fd[1]);
	}
}
