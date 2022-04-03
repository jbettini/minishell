/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:10 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 12:33:16 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *cmd, const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ",STDERR_FILENO);
		free(cmd);
	}
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	exit_error(char *cmd, const char *msg, int code)
{
	print_error(cmd, msg);
	exit(code);
}

int	error(char *cmd, const char *msg, int code)
{
	print_error(cmd, msg);
	return (code);
}

void	*error_null(char *cmd, const char *msg)
{
	print_error(cmd, msg);
	return (NULL);
}

void	error_manag(int ret)
{
	if (ret == CTRL_C)
		return ;
	if (ret == BF_ERROR)
		print_error(NULL, "file not found");
	else if (ret == OP_ERROR)
		print_error(NULL, "open() failed");
	else if (ret == DUP_ERROR)
		print_error(NULL, "dup2() failed");
	else if (ret == OUT_ERROR)
		print_error(NULL, "parse error");
	else if (ret == CMD_ERROR)
		print_error(NULL, "command not found");
	if (ret == CMD_ERROR)
		g_set.g_exit_status = 127;
	else if (ret >= BF_ERROR)
		g_set.g_exit_status = 1;
}