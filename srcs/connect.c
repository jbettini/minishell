/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/14 00:54:31 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	destroy_ftmp(void)
{
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
	// if (ft_strequ_hd(to_redir->type, "<"))
	// 	ret = redir_to_stdin(to_redir->word);
	// else if (ft_strequ_hd(to_redir->type, "<<"))
	// 	ret = redir_heredoc(to_redir->word);
	// else if (ft_strequ_hd(to_redir->type, ">>"))
	// 	ret = redir_to_stdout(to_redir->word, O_APPEND);
	// else if (ft_strequ_hd(to_redir->type, ">"))
	// 	ret = redir_to_stdout(to_redir->word, O_TRUNC);
	return (ret);
}

int	redir_all(t_list *in, t_list *out)
{
	t_redir	*to_redir;
	int		ret;

	ret = 0;
	while (in)
	{
		// ft_printf("in = %s \n", (t_redir *)in->content); // ! Test
		to_redir = (t_redir *)in->content;
		ret = redir_manag(to_redir);
		if (ret)
			return (ret);
		in = in->next;
	}
	while (out)
	{
		// ft_printf("out = %s\n", (t_redir *)out->content); // ! Test
		to_redir = (t_redir *)out->content;
		ret = redir_manag(to_redir);
		if (ret)
			return (ret);
		out = out->next;
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

int	launch_exec(t_env *env, char **arg, int mod)
{
	int	ret;

	ret = 0;
	if (mod == 1)
	{
		env->cmd_path = parse_cmd(env->path, arg);
		// ft_printf("3 launch_exec - env->cmd_path  : %s - ret = %d\n", env->cmd_path, ret); //! test
		if (!env->cmd_path && !ft_isbuild(arg[0]))
			return (CMD_ERROR);
		ft_pipex(arg, env);
		free(env->cmd_path);
	}
	else if (mod == 2)
		ret = exec_in_child(arg, env);
	else if (!mod)
		ret = exec_in_main(arg, env, 1);
	// ft_printf("3 launch_exec ret = %d\n", ret); //! test
	env->cmd_path = NULL;
	return (ret);
}

int	exec_block(t_cmd *to_exec, t_env *env, int mod)
{
	int	ret;

	ret = redir_all(to_exec->redir_in, to_exec->redir_out);
	if (ret)
		return (ret);
	ret = launch_exec(env, to_exec->args, mod);
	return (ret);
}

int	connecting_fct(t_list *line, t_env *env)
{
	int		ret;
	int		oldstdin;
	int		oldstdout;

	oldstdout = dup(1);
	oldstdin = dup(0);
	if (line->next != NULL)
	{
		while (line)
		{
			// ft_printf("1.0 connecting_fct line->content = %s\n", line->content); //! test
			if (line->next)
				ret = exec_block((t_cmd *)(line->content), env, 1);
			else
				ret = exec_block((t_cmd *)(line->content), env, 2);
			// ft_printf("1.1 connecting_fct ret = %d\n", ret); //! test
			if (ret) // ! a changer par une pipe NULL ? pour avoir result = 0
				error_manag(ret);
			line = line->next;
		}
	}
	else
		ret = exec_block((t_cmd *)(line->content), env, 0);
	error_manag(ret);
	dup2(oldstdin, 0);
	dup2(oldstdout, 1);
	destroy_ftmp();
	// ft_printf("1 connecting_fct ret (if !-1 => ret = 0) = %d\n", ret); //! test
	// if (ret == -1 || ret == CMD_ERROR)
	// 	return (ret);
	return (SUCCESS);
}
