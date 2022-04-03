/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:59:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 21:11:59 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_cpt	tmp;
	char	**res;

	init_cpt(&tmp);
	res = malloc(sizeof(char *) * (get_len_res(str, delim) + 1));
	while (str[tmp.i])
	{
		while (str[tmp.i] && is_in_set(delim, str[tmp.i]))
			tmp.i++;
		tmp.j = 0;
		while (str[tmp.i + tmp.j] && !is_in_set(delim, str[tmp.i + tmp.j]))
		{
			if (str[tmp.i + tmp.j] == '\'' || str[tmp.i + tmp.j] == '"')
				skip_quotes(str, &(tmp.j));
			tmp.j++;
		}
		if (!(tmp.j))
			break ;
		res[tmp.k++] = ft_substr(str, tmp.i, tmp.j);
		tmp.i += tmp.j;
	}
	res[tmp.k] = NULL;
	return (res);
}
