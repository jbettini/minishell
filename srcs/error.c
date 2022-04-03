/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:10 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 04:29:02 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_error(char *msg)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	exit_error(char *msg, int code)
{
	print_error(msg);
	exit(code);
}

int	error(char *msg, int code)
{
	print_error(msg);
	return (code);
}

void	*error_null(char *msg)
{
	print_error(msg);
	return (NULL);
}


void	error_manag(int ret)
{
	if (ret != CTRL_C)
	{
		if (ret == BF_ERROR)
			perror("file not found ");
		else if (ret == OP_ERROR)
			perror("file open error ");
		else if (ret == DUP_ERROR)
			perror("dup2 ret error ");
		else if (ret == OUT_ERROR)
			perror("parse error n");
		else if (ret == CMD_ERROR)
			perror("command not found ");
		if (ret == CMD_ERROR)
			g_set.g_exit_status = 127;
		else if (ret >= BF_ERROR)
			g_set.g_exit_status = 1;
	}
}