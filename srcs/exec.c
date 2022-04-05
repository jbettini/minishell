/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 14:38:36 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/05 04:36:47 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	exec_block(t_cmd *to_exec, t_env *env, int mod)
{
	int	ret;

	ret = redir_lst(to_exec->redir_in, env);
	if (ret)
		return (ret);
	if (mod == IN_MAIN)
	{
		ret = redir_lst(to_exec->redir_out, env);
		if (ret)
			return (ret);
	}
	if (to_exec->args)
	{
		set_path(env, to_exec->args, SET);
		ret = launch_exec(env, to_exec, mod);
		set_path(env, to_exec->args, DESTROY_SET);
	}
	else if (mod != IN_MAIN)
	{
		ret = redir_lst(to_exec->redir_out, env);
		if (ret)
			return (ret);
	}
	return (ret);
}

int	execute_cmd(char **args, t_env *env, int mod)
{
	int	ret;

	ret = 0;
	if (mod != IN_MAIN)
		ret = ft_cmd(args, env);
	else if (mod == IN_MAIN)
		ret = 2;
	if (ret == 2 && !env->cmd_path)
	{
		ret = CMD_ERROR;
		error_manag(ret);
	}
	else if (ret == 2)
		execve(env->cmd_path, args, env->nbtfke);
	exit(ret);
}

int	exec_in_main(t_cmd *cmd, t_env *env, int mod)
{
	int	ret;

	ret = 0;
	if (mod == IN_MAIN)
	{
		ret = ft_cmd(cmd->args, env);
		if (ret != 2)
			g_set.g_exit_status = ret;
		else if (ret == 0 && ft_strequ_hd(cmd->args[0], "unset"))
			check_unset_path(&cmd->args[1], env);
		else if (ret == 2 && !env->cmd_path)
			error_manag(CMD_ERROR);
		else if (ret == 2)
			exec_in_child(cmd->args, env, mod);
	}
	else if (mod == IN_PIPE)
	{
		ret = redir_lst(cmd->redir_out, env);
		if (ret)
			exit(1);
		execute_cmd(cmd->args, env, mod);
	}
	return (ret);
}

int	exec_in_child(char **args, t_env *env, int mod)
{
	int		pid;

	pid = fork();
	if (!pid)
	{
		reset_tty();
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		execute_cmd(args, env, mod);
	}
	else
		env->child++;
	return (0);
}
