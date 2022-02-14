/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 20:39:16 by jbettini          #+#    #+#             */
/*   Updated: 2021/11/04 20:39:44 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr(char const *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		write(1, &s[i++], 1);
}

void	ft_putlst(t_list *lst)
{
	while (lst)
	{
		ft_putstr(lst->content);
		lst = lst->next;
	}
}
