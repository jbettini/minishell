/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 15:20:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 03:24:12 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// insert src in dst at index n, only dst[n] is deleted
char	*str_insert(char *dst, const char *src, size_t n)
{
	int		i;
	char	*new;

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
