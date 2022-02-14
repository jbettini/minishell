/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap_content.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 18:40:02 by jbettini          #+#    #+#             */
/*   Updated: 2021/12/16 16:42:36 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_swap_content(t_list **a, t_list **b)
{
	void	*x;

	x = (*a)->content;
	(*a)->content = (*b)->content;
	(*b)->content = x;
}
