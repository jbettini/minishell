/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 18:55:54 by rahmed            #+#    #+#             */
/*   Updated: 2022/02/13 17:35:09 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_signal(int fd)
{
	struct termios		term;
	struct sigaction	action;
	sigset_t			set;

	tcgetattr(fd, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(fd, TCSANOW, &term);
	signal(SIGQUIT, SIG_IGN); // * Interruption forte (Ctrl-\) to IGNORE
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	action.sa_handler = signal_handler;
	action.sa_mask = set;
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL); // * Interruption (Ctrl-c)
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		//! add code for exit status on global var
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	handle_eof_ctrl_d(char *str) // * Get Ctrl-D
{
	if (str == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	return (0);
}
