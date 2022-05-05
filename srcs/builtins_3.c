/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 17:35:20 by ydanset           #+#    #+#             */
/*   Updated: 2022/05/05 17:35:32 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putendl_export(char const *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		ft_putchar(s[i++]);
	if (!s[i])
	{
		ft_putchar('\n');
		return ;
	}
	ft_putstr("=\"");
	while (s[++i])
		ft_putchar(s[i]);
	ft_putstr("\"\n");
}

void	ft_putexport(t_list *lst)
{
	while (lst)
	{
		ft_putstr("declare -x ");
		ft_putendl_export(lst->content);
		lst = lst->next;
	}
}
