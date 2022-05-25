/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 17:35:20 by ydanset           #+#    #+#             */
/*   Updated: 2022/05/25 13:32:37 by ydanset          ###   ########.fr       */
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

int	check_echo_option(char *str)
{
	int	i;

	if (!str || ft_strncmp("-n", str, 2))
		return (0);
	i = 1;
	while (str[++i])
		if (str[i] != 'n')
			return (0);
	return (1);
}

void	ft_echo(char **arg)
{
	int	i;
	int	has_option_n;

	has_option_n = check_echo_option(arg[1]);
	i = 0;
	if (has_option_n)
		i = 1;
	while (arg[++i])
	{
		ft_putstr_fd(arg[i], 1);
		if (arg[i + 1])
			ft_putchar_fd(' ', 1);
	}
	if (!has_option_n)
		ft_putchar_fd('\n', 1);
}
