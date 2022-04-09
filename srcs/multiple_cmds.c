/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/09 08:53:24 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipe_routine(t_cmd *cmd, t_env *env)
{
    int ret;

    // ret = redir_all(cmd, env);       BETTER_SPEED_PROCESS_BUT_NEED_TO_WORK_IT
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

void    wait_last_pid(t_env *env)
{
    int	i;
	int	status;
	int	x;

	x = 0;
	i = -1;
	status = 0;
	waitpid(env->last_pid, &status, 0);
	if (WIFSIGNALED(status) && !x)
	{
        x = 1;
		if (WTERMSIG(status) == SIGQUIT)
		write(STDOUT_FILENO, "Quit: 3", 7);
		write(STDOUT_FILENO, "\n", 1);
		g_set.g_exit_status = 128 + WTERMSIG(status);
	}
	if (!x)
		g_set.g_exit_status = status % 255;
    env->last_pid = 0;
}

void    reset_routine_mc(t_env *env, int mod)
{
	ft_free_split(env->nbtfke);
	env->nbtfke = ft_lst_to_dpt(env->envp);
	dup2(env->oldstdin, 0);
	dup2(env->oldstdout, 1);
	close(env->oldstdin);
	close(env->oldstdout);
	if (!access(".heredoc_tmp", F_OK))
		unlink(".heredoc_tmp");
    unlink_all(env);
	if (env->child)
		wait_this_fk_process(env);
    if (env->last_pid != 0)
        wait_last_pid(env);
    if (mod == CTRL_C)
        g_set.g_exit_status = 1;
    if (g_set.g_check_hd == 1)
        g_set.g_check_hd = 0;
	env->child = 0;
}

void	ft_pipex(t_cmd *cmd, t_env *env)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return (perror("pipe failed: "));
	pid = fork();
	if (!pid)
	{
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		pipe_routine(cmd, env);
	}
	else
	{
		env->child++;
		dup2(fd[0], 0);
		close(fd[1]);
		close(fd[0]);
	}
}

void    exec_last_pipe(t_cmd *cmd, t_env *env)
{
    int pid;

    pid = fork();
    if (!pid)
    {
       	set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL); 
        pipe_routine(cmd, env);
    }
    else
    {
        env->last_pid = pid;
        reset_routine_mc(env, PIPE);
    }
}

int	exec_multiple_cmds(t_list *cmds, t_env *env)
{
	t_cmd	*cmds_tmp;

	while (cmds)
	{
		cmds_tmp = (t_cmd *)(cmds->content);
		if (!expand_ev(cmds_tmp, env))
			cmds = cmds->next;
        else
        {
        	set_path(env, cmds_tmp->args, SET);
            if (hd_to_infile(cmds, env) == CTRL_C)
            {
                set_path(env, NULL, DESTROY_SET);
    			reset_routine_mc(env, CTRL_C);
                return (CTRL_C);
    		}
            else
            {
    		    if (cmds->next)
    			    ft_pipex(cmds_tmp, env);
    		    else
    			    exec_last_pipe(cmds_tmp, env);
    		    cmds = cmds->next;
            }
    		set_path(env, NULL, DESTROY_SET);
        }
	}
	reset_routine_mc(env, 0);
    return (SUCCESS);
}