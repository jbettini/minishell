/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:59:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 04:00:49 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_in_set(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
		if (str[i++] == c)
			return (1);
	return (0);
}

int	get_len_res(char *str, char *delim)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && is_in_set(delim, str[i]))
			i++;
		if (str[i])
			len++;
		while (str[i] && !is_in_set(delim, str[i]))
			i++;
	}
	return (len);
}

void	skip_quotes(char *str, int *j)
{
	char	quote;

	quote = str[(*j)++];
	while (str[*j] != quote)
		*j += 1;
}

char	**ft_strtok(char *str, char *delim)
{
	int		i;
	int		j;
	int		k;
	char	**res;

	res = malloc(sizeof(char *) * (get_len_res(str, delim) + 1));
	i = 0;
	k = 0;
	while (str[i])
	{
		while (str[i] && is_in_set(delim, str[i]))
			i++;
		j = 0;
		while (str[i + j] && !is_in_set(delim, str[i + j]))
		{
			if (str[i + j] == '\'' || str[i + j] == '"')
				skip_quotes(str, &j);
			j++;
		}
		if (!j)
			break ;
		res[k++] = ft_substr(str, i, j);
		i += j;
	}
	res[k] = NULL;
	return (res);
}
