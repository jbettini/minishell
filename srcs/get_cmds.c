/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:28 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/01 15:04:34 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// #include "cmds.h"

static void	init_redir(t_redir **redir, int type)
{
	*redir = malloc(sizeof(t_redir) * 1);
	(*redir)->type = type;
	(*redir)->word = NULL;
}

static int	add_redir(t_cmd *cmd, t_list **tokens)
{
	t_list	*new;
	t_redir	*redir;

	init_redir(&redir, get_token_type((*tokens)->content));
	*tokens = (*tokens)->next;
	if (!*tokens || get_token_type((*tokens)->content) != WORD)
	{
		free(redir);
		return (error("missing word after redirection", 0));
	}
	redir->word = ft_strdup(get_token_value((*tokens)->content));
	new = ft_lstnew(redir);
	if (redir->type == REDIR_LL || redir->type == REDIR_L)
		ft_lstadd_back(&cmd->redir_in, new);
	else
		ft_lstadd_back(&cmd->redir_out, new);
	return (1);
}

static void	init_cmd(t_cmd **cmd)
{
	*cmd = malloc(sizeof(t_cmd) * 1);
	(*cmd)->args = NULL;
	(*cmd)->redir_in = NULL;
	(*cmd)->redir_out = NULL;
}

static t_cmd	*get_next_cmd(t_list **tokens)
{
	t_cmd	*cmd;

	init_cmd(&cmd);
	while (*tokens && get_token_type((*tokens)->content) != PIPE)
	{
		if (get_token_type((*tokens)->content) == WORD)
			cmd->args = strs_append(cmd->args, \
					get_token_value((*tokens)->content));
		else if (!add_redir(cmd, tokens))
		{
			free_cmd(cmd);
			return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	if (*tokens)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || get_token_type((*tokens)->content) == PIPE)
		{
			free_cmd(cmd);
			return (error_null("unexpected token '|'"));
		}
	}
	return (cmd);
}

t_list	*get_cmds(t_list *tokens)
{
	t_list	*cmds;
	t_list	*new;
	t_cmd	*cmd;

	cmds = NULL;
	while (tokens)
	{
		cmd = get_next_cmd(&tokens);
		if (!cmd)
		{
			ft_lstclear(&cmds, &free_cmd);
			return (NULL);
		}
		new = ft_lstnew(cmd);
		ft_lstadd_back(&cmds, new);
	}
	return (cmds);
}
