/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:55 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 17:36:06 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_symbol(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

void	skip_whitespace(char **line)
{
	while (is_whitespace(**line))
		(*line)++;
}

void	my_strncpy(char *dst, const char *src, int n)
{
	int	i;

	if (!dst || !src || n < 0)
		return ;
	i = 0;
	while (src[i] && i < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int	my_strcmp(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

// insert src in dst at index n, only dst[n] is deleted
char	*str_insert(char *dst, const char *src, size_t n)
{
	int		i;
	char	*new;

// int		j; //! unused
	new = malloc(ft_strlen(dst) + ft_strlen(src));
	if (!new)
		return (NULL);
	my_strncpy(new, dst, n);
	i = 0;
	while (src[i])
	{
		new[n + i] = src[i];
		i++;
	}
	while (dst[n])
	{
		new[n + i] = dst[n + 1];
		n++;
	}
	free(dst);
	return (new);
}

// delete len char at index start of str and rearrange the string
char	*get_str_truncated(const char *str, int start, int len)
{
	int		i;
	char	*new;

	new = malloc(ft_strlen(str) - len + 1);
	if (!new)
		return (NULL);
	my_strncpy(new, str, start);
	i = 0;
	while (str[start + len + i])
	{
		new[start + i] = str[start + len + i];
		i++;
	}
	new[start + i] = '\0';
	return (new);
}

// delete len char at index start of str and rearrange the string, str is freed
char	*trunc_str(char *str, int start, int len)
{
	int		i;
	char	*new;

	new = malloc(ft_strlen(str) - len + 1);
	if (!new)
		return (NULL);
	my_strncpy(new, str, start);
	i = 0;
	while (str[start + len + i])
	{
		new[start + i] = str[start + len + i];
		i++;
	}
	new[start + i] = '\0';
	free(str);
	return (new);
}

int	get_token_type(t_token *tok)
{
	return (tok->type);
}

char	*get_token_value(t_token *tok)
{
	return (tok->val);
}
