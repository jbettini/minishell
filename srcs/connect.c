/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 04:28:43 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
				;
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
