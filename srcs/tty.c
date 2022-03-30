/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 16:20:27 by ydanset           #+#    #+#             */
/*   Updated: 2022/03/30 16:39:55 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_tty(void)
{
	struct termios	term;
	int				tty_fd;

	tty_fd = -1;
	if (isatty(STDIN_FILENO))
		tty_fd = STDIN_FILENO;
	else if (isatty(STDOUT_FILENO))
		tty_fd = STDOUT_FILENO;
	else if (isatty(STDERR_FILENO))
		tty_fd = STDERR_FILENO;
	if (tty_fd != -1)
	{
		tcgetattr(tty_fd, &term);
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(tty_fd, TCSANOW, &term);
	}
}

void	reset_tty(void)
{
	struct termios	term;
	int				tty_fd;

	tty_fd = -1;
	if (isatty(STDIN_FILENO))
		tty_fd = STDIN_FILENO;
	else if (isatty(STDOUT_FILENO))
		tty_fd = STDOUT_FILENO;
	else if (isatty(STDERR_FILENO))
		tty_fd = STDERR_FILENO;
	if (tty_fd != -1)
	{
		tcgetattr(tty_fd, &term);
		term.c_lflag |= ECHOCTL;
		tcsetattr(tty_fd, TCSANOW, &term);
	}
}
