/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/05 04:33:08 by jbettini         ###   ########.fr       */
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

int	exec_multiple_cmds(t_list *cmds, t_env *env)
{
	int	ret;

	while (cmds)
	{
		if (!expand_ev((t_cmd *)(cmds->content), env))
			continue ;
		if (cmds->next)
		{
			ret = exec_block((t_cmd *)(cmds->content), env, IN_PIPE);
			error_manag(ret);
		}
		else
			ret = exec_block((t_cmd *)(cmds->content), env, LAST_PIPE_BLOCK);
		if (ret == CTRL_C)
		{
			reset_routine(env, ret);
			return (0); // must handle ret
		}
		cmds = cmds->next;
	}
	reset_routine(env, ret);
	error_manag(ret);
	return (ret);
}

int	exec_cmds(t_list *cmds, t_env *env)
{
	int	ret;

	ret = 0;
	if (cmds->next)
		ret = exec_multiple_cmds(cmds, env);
	else
		ret = exec_simple_cmd((t_cmd *)(cmds->content), env);
	return (ret);
}

/*
int	connecting_fct(t_list *cmd, t_env *env)
{
	int	ret;

	ret = 0;
	if (cmd->next != NULL)
	{
		while (cmd)
		{
			if (manag_exec_in_env(cmd))
			{
				if (cmd->next)
				{
					cmd = cmd->next;
					continue ;	
				}
				else
				{
					reset_routine(env, ret);
					g_set.g_exit_status = CMD_ERROR;
					return (SUCCESS);
				}
			}
			if (!expand_ev(cmd, env))
				continue ;
			else if (cmd->next)
			{
				ret = exec_block((t_cmd *)(cmd->content), env, IN_PIPE);
				error_manag(ret);
			}
			else
				ret = exec_block((t_cmd *)(cmd->content), env, LAST_PIPE_BLOCK);
			if (ret == CTRL_C)
			{
				reset_routine(env, ret);
				return (SUCCESS);
			}
			cmd = cmd->next;
		}
		cette_fct_sert_pour_la_norm(env, IN_CHILD, ret);
	}
	else if (expand_ev(cmd, env))
	{
		if (!manag_exec_in_env(cmd))
		{
			ret = exec_block((t_cmd *)(cmd->content), env, IN_MAIN);
			cette_fct_sert_pour_la_norm(env, IN_MAIN, ret);
		}
		else
		{
			reset_routine(env, IN_MAIN);
			g_set.g_exit_status = CMD_ERROR;
		}
	}
	return (SUCCESS);
}
*/