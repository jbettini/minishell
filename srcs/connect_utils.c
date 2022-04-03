/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 08:54:58 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (g_set.g_exit_status)
		 g_set.g_exit_status = 0;
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
