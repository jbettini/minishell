/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 01:39:22 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/01 13:20:01 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(char **arg)
{
	int	i;

	i = ft_strequ_hd(arg[1], "-n");
	if (!arg[1] || (!arg[2] && ft_is_str_blank(arg[1])))
		return (ft_putchar_fd('\n', 1));
	while (arg[++i])
	{
		ft_putstr_fd(arg[i], 1);
		if (arg[i + 1])
			ft_putchar_fd(' ', 1);
	}
	if (!ft_strequ_hd(arg[1], "-n"))
		ft_putchar_fd('\n', 1);
}
