/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/08 16:38:06 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_routine(t_cmd *cmd, t_var *var)
{
	int	ret;

	ret = redir_all(cmd, var);
	if (ret)
		exit(ret);
	if (!(cmd->args))
		exit(0);
	cmd->args = expand_args(cmd->args, var);
	if (ft_isbuild(cmd->args[0]))
		exit(exec_build(cmd->args, var));
	else
	{
		if (var->cmd_path)
			execve(var->cmd_path, cmd->args, var->envp);
		print_error(cmd->args[0], "command not found");
		exit(127);
	}
}

void	wait_last_pid(t_var *var)
{
	int	status;

	status = 0;
	waitpid(var->last_pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit: 3", 7);
		write(STDOUT_FILENO, "\n", 1);
		g_glb.exit_status = 128 + WTERMSIG(status);
	}
	else
		g_glb.exit_status = status % 255;
	while (wait(NULL) > 0)
		;
}

void	reset_routine_mc(t_var *var, int mode)
{
	ft_free_split(var->envp);
	var->envp = ft_lst_to_dpt(var->local_env);
	wait_last_pid(var);
	if (mode == CTRL_C)
		g_glb.exit_status = 1;
	if (g_glb.sigint_in_hd == 1)
		g_glb.sigint_in_hd = 0;
	var->child = 0;
	unlink_all(var);
	dup2(var->oldstdin, 0);
	dup2(var->oldstdout, 1);
	close(var->oldstdin);
	close(var->oldstdout);
}

void	_pipex(t_list *cmds, t_var *var)
{
	int	pipefd[2];

	if (!cmds)
	{
		if (var->in != STDIN_FILENO)
			close(var->in);
		return ;
	}
	else if (cmds->next)
	{
		pipe(pipefd);
		var->out = pipefd[1];
	}
	else
		var->out = STDOUT_FILENO;
	set_path(var, ((t_cmd *)cmds->content)->args, SET);
	var->pid = fork();
	if (var->pid == 0)
		exec_in_pipe_child(cmds, var, pipefd[0]);
	set_path(var, NULL, DESTROY_SET);
	if (!cmds->next)
		var->last_pid = var->pid;
	if (cmds->next)
		set_next_pipe(var, pipefd);
	return (_pipex(cmds->next, var));
}

int	exec_pipe(t_list *cmds, t_var *var)
{
	int	ret;

	ret = 0;
	var->in = STDIN_FILENO;
	var->out = STDOUT_FILENO;
	set_sig(SIGINT, SIG_IGN);
	_pipex(cmds, var);
	reset_routine_mc(var, 0);
	return (SUCCESS);
}
