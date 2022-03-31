/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/31 18:54:55 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_path(t_env *env, char **args, int mod)
{
	if (mod != DESTROY_SET)
	{
		if (ft_strchr(args[0], '/'))
			env->cmd_path = ft_strdup(args[0]);
		else
			env->cmd_path = parse_cmd(env->path, args);
		return ;
	}
	if (env->cmd_path)
		free(env->cmd_path);
}

void	reset_routine(t_env *env, int ret)
{
	ret = 0;
	ft_free_split(env->nbtfke);
	env->nbtfke = ft_lst_to_dpt(env->envp);
	dup2(env->oldstdin, 0);
	dup2(env->oldstdout, 1);
	close(env->oldstdin);
	close(env->oldstdout);
	if (!access(".heredoc_tmp", F_OK))
		unlink(".heredoc_tmp");
	wait_this_fk_process(env);
	// printf("global %lld\n", g_exit_status);
	env->child = 0;
}

int	redir_manag(t_redir *to_redir, t_env *env)
{
	int	ret;

	ret = 0;
	if (to_redir->type == REDIR_L)
		ret = redir_to_stdin(to_redir->word);
	else if (to_redir->type == REDIR_LL)
	{
		dup2(env->oldstdin, 0);
		ret = redir_heredoc(to_redir->word);
	}
	else if (to_redir->type == REDIR_RR)
		ret = redir_to_stdout(to_redir->word, O_APPEND);
	else if (to_redir->type == REDIR_R)
		ret = redir_to_stdout(to_redir->word, O_TRUNC);
	return (ret);
}

int	redir_lst(t_list *redir_lst, t_env *env)
{
	t_redir	*to_redir;
	int		ret;

	ret = 0;
	while (redir_lst)
	{
		to_redir = (t_redir *)redir_lst->content;
		ret = redir_manag(to_redir, env);
		if (ret)
			return (ret);
		redir_lst = redir_lst->next;
	}
	return (SUCCESS);
}

void	error_manag(int ret)
{
	if (ret == BF_ERROR)
		perror("file not found ");
	else if (ret == OP_ERROR)
		perror("file open error ");
	else if (ret == DUP_ERROR)
		perror("dup2 ret error ");
	else if (ret == OUT_ERROR)
		perror("parse error n");
	else if (ret == CMD_ERROR)
		perror("command not found ");
}

int	launch_exec(t_env *env, t_cmd *cmd, int mod)
{
	int	ret;

	ret = 0;
	if (!cmd->args)
		return (-42);
	if (mod == IN_PIPE)
		ft_pipex(cmd, env);
	else if (mod == LAST_PIPE_BLOCK)
	{
		ret = redir_lst(cmd->redir_out, env);
		if (ret)
			return (ret);
		ret = exec_in_child(cmd->args, env, mod);
	}
	else if (mod == IN_MAIN)
		ret = exec_in_main(cmd, env, mod);
	return (ret);
}

int	exec_block(t_cmd *to_exec, t_env *env, int mod)
{
	int	ret;

	ret = redir_lst(to_exec->redir_in, env);
	if (ret)
		return (ret);
	if (mod == IN_MAIN)
	{
		ret = redir_lst(to_exec->redir_out, env);
		if (ret)
			return (ret);
	}
	if (to_exec->args)
	{
		set_path(env, to_exec->args, SET);
		ret = launch_exec(env, to_exec, mod);
		set_path(env, to_exec->args, DESTROY_SET);
	}
	return (ret);
}

int	connecting_fct(t_list *cmd, t_env *env)
{
	int		ret;

	ret = 0;
	if (cmd->next != NULL)
	{
		while (cmd)
		{
			if (cmd->next)
				ret = exec_block((t_cmd *)(cmd->content), env, IN_PIPE);
			else
				ret = exec_block((t_cmd *)(cmd->content), env, LAST_PIPE_BLOCK);
			cmd = cmd->next;
		}
		reset_routine(env, IN_CHILD);
	}
	else
	{
		ret = exec_block((t_cmd *)(cmd->content), env, IN_MAIN);
		reset_routine(env, IN_MAIN);
	}
	return (SUCCESS);
}
