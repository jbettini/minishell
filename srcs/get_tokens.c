/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:35 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 08:54:58 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_len_word(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i] && !is_whitespace(line[i]) && !is_symbol(line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
				return (-1);
		}
		i++;
	}
	return (i);
}

static int	extract_word(char **line, t_token *tok)
{
	int		i;

	i = get_len_word(*line);
	if (i == -1)
		return (error(NULL, "quotes unclosed", 0));
	tok->type = WORD;
	tok->val = malloc(i + 1);
	my_strncpy(tok->val, *line, i);
	*line += i;
	return (1);
}

static int	extract_symbol(char **line, t_token *tok)
{
	tok->val = NULL;
	if (**line == '|')
		tok->type = PIPE;
	if (**line == '<' && *(*line + 1) == '<')
		tok->type = REDIR_LL;
	else if (**line == '<')
		tok->type = REDIR_L;
	else if (**line == '>' && *(*line + 1) == '>')
		tok->type = REDIR_RR;
	else if (**line == '>')
		tok->type = REDIR_R;
	if ((**line == '<' && *(*line + 1) == '<')
		|| (**line == '>' && *(*line + 1) == '>'))
		*line += 2;
	else
		*line += 1;
	return (1);
}

static t_token	*get_next_token(char **line)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token) * 1);
	if (is_symbol(**line))
		extract_symbol(line, tok);
	else if (!extract_word(line, tok))
	{
		free(tok);
		return (NULL);
	}
	return (tok);
}

t_list	*get_tokens(char *line)
{
	t_list	*tokens;
	t_list	*new;
	t_token	*tok;

	tokens = NULL;
	skip_whitespace(&line);
	while (*line)
	{
		tok = get_next_token(&line);
		if (!tok)
		{
			ft_lstclear(&tokens, &free_token);
			return (NULL);
		}
		new = ft_lstnew(tok);
		ft_lstadd_back(&tokens, new);
		skip_whitespace(&line);
	}
	return (tokens);
}
