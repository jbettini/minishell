/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Untitled-1                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:50:08 by jbettini          #+#    #+#             */
/*   Updated: 2021/11/19 19:03:30 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putbase(unsigned long long nb, char *str)
{
	if (nb >= ft_strlen(str))
		ft_putbase(nb / ft_strlen(str), str);
	ft_putchar(str[nb % ft_strlen(str)]);
}

int	ft_putsize(unsigned long long nb, char *base)
{
	int	i;

	i = 0;
	ft_putbase(nb, base);
	while (nb != 0)
	{
		nb /= ft_strlen(base);
		i++;
	}
	return (i);
}
