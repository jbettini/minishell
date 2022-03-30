/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_del.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 18:55:54 by rahmed            #+#    #+#             */
/*   Updated: 2022/03/29 13:38:21 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	set_mainprocess_sig(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_handler);
}

void	ignore_sigint(void)
{
	signal(SIGINT, SIG_IGN);
}

void	set_subprocess_sig(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

/*
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
*/

int	handle_eof_ctrl_d(char *str) // * Get Ctrl-D
{
	if (str == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	return (0);
}
