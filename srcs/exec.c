/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 14:38:36 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 12:39:11 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	ft_cmd(char **args, t_env *env)
{
	if (args[0] == NULL)
		return (0);
	else if (ft_strequ_hd(args[0], "exit"))
		return (ft_exit(args, 1, env));
	else if (ft_strequ_hd(args[0], "unset"))
		return (ft_unset(args, env));
	else if (ft_strequ_hd(args[0], "export"))
		return (ft_export(args, env));
	else if (ft_strequ_hd(args[0], "pwd"))
		return (ft_pwd(args));
	else if (ft_strequ_hd(args[0], "echo"))
		ft_echo(args);
	else if (ft_strequ_hd(args[0], "cd"))
		ft_cd(args, &(env->envp));
	else if (ft_strequ_hd(args[0], "env"))
		ft_env(args, env);
	else
		return (2);
	return (0);
}

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

