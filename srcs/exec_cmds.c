/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/17 12:59:34 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_path(t_var *var, char **args, int mode)
{
	if (mode != DESTROY_SET && args)
	{
		if (ft_strchr(args[0], '/'))
		{	
			if (access(args[0], F_OK | X_OK) != 0)
				var->cmd_path = 0;
			else
				var->cmd_path = ft_strdup(args[0]);
		}
		else
			var->cmd_path = parse_cmd(var->path, args);
		return ;
	}
	if (var->cmd_path && mode == DESTROY_SET)
	{
		free(var->cmd_path);
		var->cmd_path = NULL;
	}
}

void	update_path(t_var *var)
{
	t_list	*tmp;

	tmp = var->local_env;
	if (var->path)
		ft_free_split(var->path);
	var->path = NULL;
	while (tmp)
	{
		if (ft_strnequ(tmp->content, "PATH=", 5))
		{
			var->path = ft_split((tmp->content) + 5, ':');
			break ;
		}
		tmp = tmp->next;
	}
	ft_free_split(var->envp);
	var->envp = ft_lst_to_dpt(var->local_env);
}

int	exec_cmds(t_list *cmds, t_var *var)
{
	int	ret;

	ret = 0;
	tty_show_ctrl();
	if (hd_to_infile(cmds, var) == CTRL_C)
	{
		set_path(var, NULL, DESTROY_SET);
		reset_routine_mc(var, CTRL_C);
		return (CTRL_C);
	}
	if (cmds->next)
		ret = exec_pipe(cmds, var);
	else
		ret = exec_simple_cmd((t_cmd *)(cmds->content), var);
	update_path(var);
	return (ret);
}
