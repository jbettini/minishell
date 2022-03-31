/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:25:34 by ydanset           #+#    #+#             */
/*   Updated: 2022/03/31 19:11:01 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tokens(t_list *tokens)
{
	t_token	*tok;

	ft_printf("\033[1;33mTOKENS\n\033[0m");
	while (tokens)
	{
		tok = tokens->content;
		if (tok->type == WORD)
			ft_printf("\033[1;33mword: %s\n\033[0m", tok->val);
		else if (tok->type == PIPE)
			ft_printf("\033[1;33mpipe\n\033[0m");
		else if (tok->type == REDIR_L)
			ft_printf("\033[1;33m<\n\033[0m");
		else if (tok->type == REDIR_LL)
			ft_printf("\033[1;33m<<\n\033[0m");
		else if (tok->type == REDIR_R)
			ft_printf("\033[1;33m>\n\033[0m");
		else if (tok->type == REDIR_RR)
			ft_printf("\033[1;33m>>\n\033[0m");
		else
			ft_printf("\033[1;33mtoken has invalid type\n\033[0m");
		tokens = tokens->next;
	}
}

void	print_redir(void *ptr)
{
	t_redir	*redir;

	redir = ptr;
	if (redir->type == REDIR_RR)
		ft_printf("\033[1;33m>> %s\n\033[0m", redir->word);
	else if (redir->type == REDIR_R)
		ft_printf("\033[1;33m> %s\n\033[0m", redir->word);
	else if (redir->type == REDIR_LL)
		ft_printf("\033[1;33m<< %s\n\033[0m", redir->word);
	else if (redir->type == REDIR_L)
		ft_printf("\033[1;33m< %s\n\033[0m", redir->word);
	else
		print_error("redir has invalid type\n");
}

void	print_cmd(void *ptr)
{
	t_cmd	*cmd;

	cmd = ptr;
	ft_printf("\033[1;33mNEW CMD\n\033[0m");
	ft_printf("\033[1;33m--- args ---\n\033[0m");
	print_strs(cmd->args);
	ft_printf("\033[1;33m--- redir_in ---\n\033[0m");
	ft_lstiter(cmd->redir_in, &print_redir);
	ft_printf("\033[1;33m--- redir_out ---\n\033[0m");
	ft_lstiter(cmd->redir_out, &print_redir);
}
