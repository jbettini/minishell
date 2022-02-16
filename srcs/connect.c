/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/16 07:57:37 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	reset_routine(t_env *env)
{
	dup2(env->oldstdin, 0);
	dup2(env->oldstdout, 1);
	if (!access(".heredoc_tmp", F_OK))
		unlink(".heredoc_tmp");
}

int	redir_manag(t_redir *to_redir)
{
	int	ret;

	ret = 0;
	if (to_redir->type == REDIR_L)
		ret = redir_to_stdin(to_redir->word);
	else if (to_redir->type == REDIR_LL)
		ret = redir_heredoc(to_redir->word);
	else if (to_redir->type == REDIR_RR)
		ret = redir_to_stdout(to_redir->word, O_APPEND);
	else if (to_redir->type == REDIR_R)
		ret = redir_to_stdout(to_redir->word, O_TRUNC);
	return (ret);
}

int	redir_lst(t_list *redir_lst)
{
	t_redir	*to_redir;
	int		ret;

	ret = 0;
	while (redir_lst)
	{
		to_redir = (t_redir *)redir_lst->content;
		ret = redir_manag(to_redir);
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
	if (mod == 1)
	{
		env->cmd_path = parse_cmd(env->path, cmd->args);
		ft_pipex(cmd, env);
		if (env->cmd_path)
			free(env->cmd_path);
	}
	else if (mod == 2)
		ret = exec_in_child(cmd, env);
	else if (!mod)
		ret = exec_in_main(cmd, env, 1);
	// ft_printf("3 launch_exec ret = %d\n", ret); //! test
	env->cmd_path = NULL;
	return (ret);
}

int	exec_block(t_cmd *to_exec, t_env *env, int mod)
{
	int	ret;

	ret = redir_lst(to_exec->redir_in);
	if (ret)
		return (ret);
	ret = launch_exec(env, to_exec, mod);
	return (ret);
}

int	connecting_fct(t_list *cmd, t_env *env)
{
	int		ret;

	if (cmd->next != NULL)
	{
		while (cmd)
		{
			// ft_printf("1.0 connecting_fct line->content = %s\n", line->content); //! test
			if (cmd->next)
				ret = exec_block((t_cmd *)(cmd->content), env, 1);
			else
				ret = exec_block((t_cmd *)(cmd->content), env, 2);
			// ft_printf("1.1 connecting_fct ret = %d\n", ret); //! test
			if (ret) // ! a changer par une pipe NULL ? pour avoir result = 0
				error_manag(ret);
			cmd = cmd->next;
		}
	}
	else
		ret = exec_block((t_cmd *)(cmd->content), env, 0);
	error_manag(ret);
	reset_routine(env);
	// ft_printf("1 connecting_fct ret (if !-1 => ret = 0) = %d\n", ret); //! test
	if (ret == -1)
		return (ret);
	return (SUCCESS);
}
