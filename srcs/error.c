/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:10 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/05 06:02:05 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *cmd, const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		//free(cmd);
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
int	all_error(int ret, char *error)
{
	if (ret == BF_ERROR)
		print_error(error, "file not found");
	else if (ret == OP_ERROR)
		print_error(error, "open() failed");
	else if (ret == DUP_ERROR)
		print_error(error, "dup2() failed");
	else if (ret == OUT_ERROR)
		print_error(error, "parse error");
	else if (ret == CMD_ERROR)
		print_error(error, "command not found");
	else if (ret == PERM_ERROR)
		print_error(error, "Permission denied");
	return (1);
}

void	error_manag(int ret)
{
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
}
