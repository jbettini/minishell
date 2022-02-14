/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 01:39:22 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/13 18:09:09 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pipex(char **arg, t_env *env)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (!pid)
	{
		// ft_printf("\033[1;33m 3.F ft_pipex close fd0 / pid = %d\n\033[0m", pid); //! test
		dup2(fd[1], 1);
		close(fd[0]);
		exec_in_main(arg, env, 0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		// ft_printf("\033[1;33m 3.M ft_pipex close fd1 / pid = %d\n\033[0m", pid); //! test
		dup2(fd[0], 0);
		close(fd[1]);
	}
}
