/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_del_nl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 14:42:59 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/26 15:10:28 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str_del_nl(char *str)
{
	char			*new;
	unsigned long	i;

	if (!str || !ft_strlen(str) || str[ft_strlen(str) - 1] != '\n')
		return (str);
	i = -1;
	new = malloc(1 * ft_strlen(str));
	while (++i < ft_strlen(str) - 1)
		new[i] = str[i];
	new[i] = '\0';
	free(str);
	return (new);
}
