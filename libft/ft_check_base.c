/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:08:47 by jbettini          #+#    #+#             */
/*   Updated: 2021/11/19 19:09:45 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_base(char *str)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' || str[i] == '+')
			return (0);
		i++;
	}
	i = 0;
	while (str[i])
	{
		tmp = str[i];
		j = i + 1;
		while (str[j])
		{
			if (tmp == str[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_check_base(char *base)
{
	if (ft_strlen(base) > 1 && check_base(base))
		return (1);
	return (0);
}
