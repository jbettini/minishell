/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 21:46:08 by jbettini          #+#    #+#             */
/*   Updated: 2021/06/19 19:33:12 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>

int	*ft_range(int min, int max)
{
	int	*str;
	int	i;

	str = NULL;
	if (min >= max)
		return (str);
	i = 0;
	str = malloc((max - min) * sizeof(int));
	while (min < max)
	{
		str[i] = min;
		min++;
		i++;
	}
	return (str);
}
