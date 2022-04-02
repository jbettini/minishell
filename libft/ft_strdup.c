/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/27 20:05:19 by rahmed            #+#    #+#             */
/*   Updated: 2022/04/02 04:54:49 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*tab;
	int		i;
	int		lensrc;

	i = 0;
	lensrc = ft_strlen(s1);
	tab = (char *)ft_calloc((lensrc + 1), sizeof(*tab));
	if (!tab)
		return (NULL);
	while (i < lensrc)
	{
		tab[i] = s1[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

char	**ft_dupdpt(char **src)
{
	char **cpy;
	int		i;

	i = -1;
	cpy = malloc(sizeof(char *) * ft_double_strlen(src) + 1);
	if (!cpy)
		return (NULL);
	while (src[++i])
		cpy[i] = ft_strdup(src[i]);
	cpy[i] = 0;
	return (cpy);
}