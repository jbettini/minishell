/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:50 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/13 18:37:50 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
		ft_printf("\033[1;32m%s\n\033[0m", strs[i++]); // ? ADD couleurs pour debug
}

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

char	**copy_strs(char **strs)
{
	char	**new;
	int		i;

	if (!strs)
		return (NULL);
	i = 0;
	while (strs[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		new[i] = ft_strdup(strs[i]);
		if (!new[i])
			return (NULL);
		i++;
	}
	new[i] = NULL;
	return (new);
}

int	strs_len(char **strs)
{
	int	i;

	if (!strs)
		return (0);
	i = 0;
	while (strs[i])
		i++;
	return (i);
}

char	**strs_append(char **strs, const char *str)
{
	int		i;
	char	**new;

	new = malloc(sizeof(char *) * (strs_len(strs) + 2));
	if (!new)
		return (NULL);
	i = 0;
	while (strs && strs[i])
	{
		new[i] = ft_strdup(strs[i]);
		if (!new[i])
			return (NULL);
		i++;
	}
	new[i] = ft_strdup(str);
	if (!new[i])
		return (NULL);
	new[i + 1] = NULL;
	free_strs(strs);
	return (new);
}
