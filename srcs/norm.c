/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 21:02:15 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 17:10:09 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	cette_fct_sert_pour_la_norm(t_env *env, const int mod, int ret)
// {
// 	// reset_routine(env, mod);
// 	void
// 	error_manag(ret);
// }

void	*cette_fct_sert_a_normer_le_hd(t_list **lst)
{
	ft_lstclear(lst, &free);
	return (NULL);
}

int	cette_fct_seet_a_normer_minishell(void)
{
	tty_show_ctrl();
	ft_putstr_fd("exit\n", 1);
	return (-1);
}

void	init_cpt(t_cpt *cpt)
{
	cpt->i = 0;
	cpt->j = 0;
	cpt->k = 0;
	cpt->equ = 0;
	cpt->ret = 0;
}

void	exec_in_pipe_child(t_list *cmds, t_env *env, int to_close)
{
	set_sig(SIGINT, SIG_DFL);
	set_sig(SIGQUIT, SIG_DFL);
	if (cmds->next)
		close(to_close);
	dup2(env->in, STDIN_FILENO);
	dup2(env->out, STDOUT_FILENO);
	if (!expand_ev(cmds->content, env))
		exit(1);
	pipe_routine(cmds->content, env);
}

void	set_next_pipe(t_env *env, int *pipefd)
{
	if (env->in != STDIN_FILENO)
		close(env->in);
	close(pipefd[1]);
	env->in = pipefd[0];
}
