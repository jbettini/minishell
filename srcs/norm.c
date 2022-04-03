/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 21:02:15 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 21:33:40 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cette_fct_sert_pour_la_norm(t_env *env, const int mod, int ret)
{
	reset_routine(env, mod);
	error_manag(ret);
}

void	*cette_fct_sert_a_normer_le_hd(t_list **lst)
{
	set_sig(SIGINT, SIG_IGN);
	ft_lstclear(lst, &free);
	return (NULL);
}

void	cette_fct_seet_a_normer_minishell(t_env *env)
{
	env_manag(NULL, env, 1);
	reset_tty();
	ft_putstr_fd("exit\n", 1);
	exit(0);
}

void	init_cpt(t_cpt *cpt)
{
	cpt->i = 0;
	cpt->j = 0;
	cpt->k = 0;
	cpt->equ = 0;
	cpt->ret = 0;
}
