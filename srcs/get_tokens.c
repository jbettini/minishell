/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:35 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 15:10:06 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// #include "cmds.h"
// #include "libft.h"

static int	get_len_word(char *line)
{
	int		len_word;
	char	quote;

	len_word = 0;
	while (*line && !is_whitespace(*line) && !is_symbol(*line))
	{
		if (*line == '"' || *line == '\'')
		{
			quote = *line;
			line++;
			while (*line && *line != quote)
			{
				len_word++;
				line++;
			}
			if (!*line)
				return (-1);
			line++;
		}
		else
		{
			len_word++;
			line++;
		}
	}
	return (len_word);
}

static int	extract_word(char **line, t_token *tok)
{
	char	quote;
	int		i;

	i = get_len_word(*line);
	if (i == - 1)
		return (error("quotes unclosed", 0));
	tok->type = WORD;
	tok->val = malloc(i + 1);
	if (!tok->val)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	i = 0;
	while (**line && !is_whitespace(**line) && !is_symbol(**line))
	{
		if (**line == '"' || **line == '\'')
		{
			quote = **line;
			*line += 1;
			while (**line && **line != quote)
				tok->val[i++] = *((*line)++);
			*line += 1;
		}
		else
			tok->val[i++] = *((*line)++);
	}
	tok->val[i] = '\0';
	return (1);
}

static int	extract_symbol(char **line, t_token *tok)
{
	tok->val = NULL;
	if (**line == '|')
		tok->type = PIPE;
	if (**line == '<' && *(*line + 1) == '<')
	{
		tok->type = REDIR_LL;
		if (*(*line + 2) == '<')
			return (error("syntax error near unexpected token '<'", 0));
	}
	else if (**line == '<')
		tok->type = REDIR_L;
	else if (**line == '>' && *(*line + 1) == '>')
	{
		tok->type = REDIR_RR;
		if (*(*line + 2) == '>')
			return (error("syntax error near unexpected token '>'", 0));
	}
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
	if (!tok)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	if (is_symbol(**line))
	{
		if (!extract_symbol(line, tok))
		{
			free(tok);
			return (NULL);
		}
	}
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
		if (!new)
			exit_error("cannot allocate memory", EXIT_FAILURE);
		ft_lstadd_back(&tokens, new);
		skip_whitespace(&line);
	}
	return (tokens);
}
