/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 10:25:41 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
int	exec_one_cmd(t_cmd *cmd, t_env *env)
{
	
}

int	exec_multiple_cmds(t_list *cmds, t_env *env)
{
	
}
*/

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
