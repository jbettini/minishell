/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:23 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 08:54:58 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include "cmds.h"

void	free_token(void *ptr)
{
	t_token	*tok;

	tok = ptr;
	free(tok->val);
	free(tok);
}

void	free_redir(void *ptr)
{
	t_redir	*redir;

	redir = ptr;
	free(redir->keyword);
	free(redir->filename);
	free(redir);
}

void	free_cmd(void *ptr)
{
	t_cmd	*cmd;

	cmd = ptr;
	free_strs(cmd->args);
	ft_lstclear(&cmd->redirs, &free_redir);
	free(cmd);
}
