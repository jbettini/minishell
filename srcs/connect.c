/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 13:10:07 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_simple_cmd(t_cmd *cmd, t_env *env)
{
	int	ret;
	
	if (!expand_ev(cmd, env))
		return (1);
	ret = exec_block(cmd, env, IN_MAIN);
	reset_routine(env, IN_MAIN);
	error_manag(ret);
	return (ret);
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