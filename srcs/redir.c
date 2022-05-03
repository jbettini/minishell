/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 04:17:11 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/26 19:10:58 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_to_stdout(void *file, int mod)
{
	int	fd;

	fd = 0;
	if (file == NULL || ft_strequ_hd(file, "|"))
		return (OUT_ERROR);
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) == -1)
			return (PERM_ERROR);
	if (mod == O_TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0744);
	else if (mod == O_APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0744);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 1) == -1)
		return (DUP_ERROR);
	close(fd);
	return (0);
}

int	redir_to_stdin(void *file)
{
	int	fd;

	if (access(file, F_OK) == -1)
		return (BF_ERROR);
	if (access(file, R_OK) == -1)
		return (PERM_ERROR);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 0) == -1)
		return (DUP_ERROR);
	close(fd);
	return (0);
}

int	redir_manag(t_redir *to_redir)
{
	int	ret;

	ret = 0;
	if (to_redir->type == REDIR_L || to_redir->type == REDIR_LL)
		ret = redir_to_stdin(to_redir->filename);
	else if (to_redir->type == REDIR_RR)
		ret = redir_to_stdout(to_redir->filename, O_APPEND);
	else if (to_redir->type == REDIR_R)
		ret = redir_to_stdout(to_redir->filename, O_TRUNC);
	if (ret == CTRL_C)
		return (ret);
	else if (ret)
		return (all_error(ret, to_redir->filename));
	return (0);
}

int	redir_lst(t_list *redir_lst, t_env *env)
{
	t_redir	*to_redir;
	int		ret;

	ret = 0;
	while (redir_lst)
	{
		to_redir = (t_redir *)redir_lst->content;
		if (expand_redir(to_redir, env))
			return (1);
		ret = redir_manag(to_redir);
		if (ret)
			return (ret);
		redir_lst = redir_lst->next;
	}
	return (0);
}

int	redir_all(t_cmd *cmd, t_env *env)
{
	int	ret;

	ret = redir_lst(cmd->redirs, env);
	if (ret)
		return (ret);
	return (0);
}
