/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 04:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/07 02:26:28 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmds(t_list *cmds, t_env *env)
{
	int	ret;

	ret = 0;
	if (cmds->next)
		ret = exec_multiple_cmds(cmds, env);
	else
		ret = exec_simple_cmd((t_cmd *)(cmds->content), env);
	return (ret);
}