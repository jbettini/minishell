/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/02 05:07:28 by jbettini         ###   ########.fr       */
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
	if (ret == CMD_ERROR)
		g_exit_status = 127;
	else if (ret >= BF_ERROR)
		g_exit_status = 1;
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
	else if (mod != IN_MAIN)
	{
		ret = redir_lst(to_exec->redir_out, env);
		if (ret)
			return (ret);
	}
	return (ret);
}

void	cette_fct_sert_pour_la_norm(t_env *env, const int mod, int ret)
{
	reset_routine(env, mod);
	error_manag(ret);
}

int		check_the_build_for_env(char *args)
{
	if (ft_strequ_hd(args, "unset"))
		ft_putstr("env: export: No such file or directory\n");
	else if (ft_strequ_hd(args, "export"))
		ft_putstr("env: export: No such file or directory\n");
	else if (ft_strequ_hd(args, "exit"))
		ft_putstr("env: exit: No such file or directory\n");
	else
		return (0);
	return (1);
}

int	manag_exec_in_env(t_list *cmd)
{
	char	**args;
	char	**new_args;
	int		i;

	i = 0;
	new_args = NULL;
	args = ((t_cmd *)cmd->content)->args;
	if (!ft_strequ_hd(args[0], "env"))
		return (0);
	while (ft_strequ_hd(args[i], "env") && args[i - 1])
		i++;
	if (check_the_build_for_env(args[i]))
		return (1);
	new_args = ft_dupdpt(&args[i]);
	ft_free_split(args);
	((t_cmd *)cmd->content)->args = new_args;
	return (0);	
}

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
				cmd = cmd->next;
				continue ;
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
			reset_routine(env, IN_MAIN);
	}
	return (SUCCESS);
}
