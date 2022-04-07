/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/07 01:37:29 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_build(char **args, t_env *env)
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
	return (0);
}

int redir_all(t_cmd *cmd, t_env *env)
{
    int ret;

    ret = redir_lst(cmd->redir_in, env);
    if (ret)
        return (ret);
    ret = redir_lst(cmd->redir_out, env);
    if (ret)
        return (ret);
    return (0);
}

void	exec_cmd_sc(char **args, t_env *env)
{
    int		pid;

    set_path(env, args, SET);
	pid = fork();
	if (!pid)
	{
		reset_tty();
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
        if (env->cmd_path)
            execve(env->cmd_path, args, env->nbtfke);
        exit(127);
	}
	else
    {
        set_path(env, args, DESTROY_SET);
		env->child++;
    }
}

void    reset_routine_sc(t_env *env, int ret)
{
	ft_free_split(env->nbtfke);
	env->nbtfke = ft_lst_to_dpt(env->envp);
	dup2(env->oldstdin, 0);
	dup2(env->oldstdout, 1);
	close(env->oldstdin);
	close(env->oldstdout);
	if (!access(".heredoc_tmp", F_OK))
		unlink(".heredoc_tmp");
	if (env->child)
		wait_this_fk_process(env);
	else
		g_set.g_exit_status = ret;
	if (ret == CTRL_C)
		g_set.g_exit_status = 1;
	env->child = 0;
	if (g_set.g_check_hd)
		g_set.g_check_hd = 0;
}
int	exec_simple_cmd(t_cmd *cmd, t_env *env)
{
    int ret;
    
	if (!expand_ev(cmd, env))
		ret = EXPAND_ERROR;
	ret = redir_all(cmd, env);
	if (!ret && cmd->args)
	{
		if (ft_isbuild(cmd->args[0]))
		{
			ret = exec_build(cmd->args, env);
			check_unset_path(cmd->args, env);
		}	
		else
			exec_cmd_sc(cmd->args, env);
	}
	reset_routine_sc(env, ret);
	if (g_set.g_exit_status == CMD_ERROR)
		all_error(CMD_ERROR, cmd->args[0]);
	return (ret);
}

