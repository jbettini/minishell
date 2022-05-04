/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 18:57:57 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_build(char **args, t_var *var)
{
	if (args[0] == NULL)
		return (0);
	else if (ft_strequ_hd(args[0], "exit"))
		return (ft_exit(args, 1, var));
	else if (ft_strequ_hd(args[0], "unset"))
		return (ft_unset(args, var));
	else if (ft_strequ_hd(args[0], "export"))
		return (ft_export(args, var));
	else if (ft_strequ_hd(args[0], "pwd"))
		return (ft_pwd(args));
	else if (ft_strequ_hd(args[0], "echo"))
		ft_echo(args);
	else if (ft_strequ_hd(args[0], "cd"))
		ft_cd(args, var);
	else if (ft_strequ_hd(args[0], "env"))
		ft_env(args, var);
	return (0);
}

void	exec_cmd_sc(char **args, t_var *var)
{
	int	pid;

	set_path(var, args, SET);
	pid = fork();
	if (!pid)
	{
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		if (var->cmd_path)
			execve(var->cmd_path, args, var->envp);
		exit(127);
	}
	else
	{
		set_path(var, args, DESTROY_SET);
		var->child++;
	}
}

void	reset_routine_sc(t_var *var, int ret)
{
	ft_free_split(var->envp);
	var->envp = ft_lst_to_dpt(var->local_env);
	dup2(var->oldstdin, 0);
	dup2(var->oldstdout, 1);
	close(var->oldstdin);
	close(var->oldstdout);
	if (!access(".heredoc_tmp", F_OK))
		unlink(".heredoc_tmp");
	if (var->child)
		wait_this_fk_process(var);
	else
		g.exit_status = ret;
	if (ret == CTRL_C)
		g.exit_status = 1;
	var->child = 0;
	if (g.sigint_in_hd)
		g.sigint_in_hd = 0;
}

int	exec_simple_cmd(t_cmd *cmd, t_var *var)
{
	int	ret;

	ret = redir_all(cmd, var);
	cmd->args = expand_args(cmd->args, var);
	set_sig(SIGINT, SIG_IGN);
	if (!ret && cmd->args)
	{
		if (ft_isbuild(cmd->args[0]))
		{
			ret = exec_build(cmd->args, var);
			check_unset_path(cmd->args, var);
		}
		else
			exec_cmd_sc(cmd->args, var);
	}
	reset_routine_sc(var, ret);
	if (g.exit_status == CMD_ERROR)
		all_error(CMD_ERROR, cmd->args[0]);
	return (ret);
}
