/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:44 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 15:10:27 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// #include "cmds.h"

t_list	*parse(char *line)
{
	t_list	*tokens;
	t_list	*cmds;

	tokens = get_tokens(line);
	if (!tokens)
		return (NULL);
	cmds = get_cmds(tokens);
	ft_lstclear(&tokens, &free_token);
	return (cmds);
}
