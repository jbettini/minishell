/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:23 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 19:10:43 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
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
	free(redir->word);
	free(redir);
}

void	free_cmd(void *ptr)
{
	t_cmd	*cmd;

	cmd = ptr;
	free_strs(cmd->args);
	ft_lstclear(&cmd->redir_in, &free_redir);
	ft_lstclear(&cmd->redir_out, &free_redir);
	free(cmd);
}
