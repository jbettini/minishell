/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 17:02:01 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_path(t_env *env, char **args, int mod)
{
	if (mod != DESTROY_SET && args)
	{
		if (ft_strchr(args[0], '/'))
		{	
			if (access(args[0], F_OK | X_OK) != 0)
				env->cmd_path = 0;
			else
				env->cmd_path = ft_strdup(args[0]);
		}
		else
			env->cmd_path = parse_cmd(env->path, args);
		return ;
	}
	if (env->cmd_path && mod == DESTROY_SET)
	{
		free(env->cmd_path);
		env->cmd_path = NULL;
	}
}

int	exec_cmds(t_list *cmds, t_env *env)
{
	int	ret;

	ret = 0;
	tty_show_ctrl();
	if (hd_to_infile(cmds, env) == CTRL_C)
	{
		set_path(env, NULL, DESTROY_SET);
		reset_routine_mc(env, CTRL_C);
		return (CTRL_C);
	}
	if (cmds->next)
		ret = exec_pipe(cmds, env);
	else
		ret = exec_simple_cmd((t_cmd *)(cmds->content), env);
	return (ret);
}
