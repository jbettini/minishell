/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 17:17:16 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_routine(t_cmd *cmd, t_env *env)
{
	int	ret;

	ret = redir_all(cmd, env);
	if (ret)
		exit(ret);
	if (!(cmd->args))
		exit(0);
	if (ft_isbuild(cmd->args[0]))
		exit(exec_build(cmd->args, env));
	else
	{
		if (env->cmd_path)
			execve(env->cmd_path, cmd->args, env->nbtfke);
		print_error(cmd->args[0], "command not found");
		exit(127);
	}
}

void	wait_last_pid(t_env *env)
{
	int	status;

	status = 0;
	waitpid(env->last_pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit: 3", 7);
		write(STDOUT_FILENO, "\n", 1);
		g.exit_status = 128 + WTERMSIG(status);
	}
	else
		g.exit_status = status % 255;
	while (wait(NULL) > 0)
		;
}

void	reset_routine_mc(t_env *env, int mod)
{
	ft_free_split(env->nbtfke);
	env->nbtfke = ft_lst_to_dpt(env->envp);
	wait_last_pid(env);
	if (mod == CTRL_C)
		g.exit_status = 1;
	if (g.hd_exited_from_sigint == 1)
		g.hd_exited_from_sigint = 0;
	env->child = 0;
	unlink_all(env);
	if (mod == CTRL_C)
		dup2(env->oldstdin, 0);
}

void	_pipex(t_list *cmds, t_env *env)
{
	int	pipefd[2];

	if (!cmds)
	{
		if (env->in != STDIN_FILENO)
			close(env->in);
		return ;
	}
	else if (cmds->next)
	{
		pipe(pipefd);
		env->out = pipefd[1];
	}
	else
		env->out = STDOUT_FILENO;
	set_path(env, ((t_cmd *)cmds->content)->args, SET);
	env->pid = fork();
	if (env->pid == 0)
		exec_in_pipe_child(cmds, env, pipefd[0]);
	set_path(env, NULL, DESTROY_SET);
	if (!cmds->next)
		env->last_pid = env->pid;
	if (cmds->next)
		set_next_pipe(env, pipefd);
	return (_pipex(cmds->next, env));
}

int	exec_pipe(t_list *cmds, t_env *env)
{
	int	ret;

	ret = 0;
	if (hd_to_infile(cmds, env) == CTRL_C)
	{
		set_path(env, NULL, DESTROY_SET);
		reset_routine_mc(env, CTRL_C);
		return (CTRL_C);
	}
	env->in = STDIN_FILENO;
	env->out = STDOUT_FILENO;
	set_sig(SIGINT, SIG_IGN);
	_pipex(cmds, env);
	reset_routine_mc(env, 0);
	return (SUCCESS);
}
