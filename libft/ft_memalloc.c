/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 20:25:51 by jbettini          #+#    #+#             */
/*   Updated: 2021/11/04 20:26:52 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*pt;
	size_t	i;

	i = 0;
	pt = malloc(sizeof(size));
	while (i < size)
		((unsigned char *)pt)[i++] = 0;
	return (pt);
}
