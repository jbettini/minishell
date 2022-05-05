/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 17:31:46 by ydanset           #+#    #+#             */
/*   Updated: 2022/05/05 17:34:27 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned long long	ft_atoull(char *nbr)
{
	unsigned long long	res;

	res = 0;
	while (ft_isdigit(*nbr))
		res = res * 10 + ((int)*(nbr++) - 48);
	return (res);
}

static int	is_valid_nbr(char *nbr)
{
	int					neg;
	unsigned long long	n;

	if (!ft_str_isdigit(nbr))
		return (0);
	neg = 0;
	if (*nbr == '-')
	{
		neg = 1;
		nbr++;
	}
	if (ft_strlen(nbr) > 19)
		return (0);
	n = ft_atoull(nbr);
	if ((neg && n > (unsigned long long)LONG_MAX + 1) || (!neg && n > LONG_MAX))
		return (0);
	return (1);
}

int	ft_exit(char **args, int print_exit, t_var *var)
{
	if (print_exit)
		ft_putstr_fd("exit\n", 1);
	if (args[1] && !is_valid_nbr(args[1]))
	{
		print_error(ft_strdup("exit"), "numeric argument required");
		var_manager(NULL, var, 1);
		exit(255);
	}
	else if (ft_double_strlen(args) > 2)
		print_error(ft_strdup("exit"), "too many arguments");
	else
	{
		if (args[1])
		{
			var_manager(NULL, var, 1);
			exit(ft_atoll(args[1]));
		}
		var_manager(NULL, var, 1);
		exit(0);
	}
	return (BUILD_ERROR);
}
