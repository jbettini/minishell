/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 17:17:44 by ydanset           #+#    #+#             */
/*   Updated: 2022/05/10 17:46:40 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_prompt(t_var *var)
{
	char	*pwd;
	char	*prompt;
	int		i;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		prompt = ft_strdup("minishell");
	else
	{
		i = ft_strlen(pwd);
		while (i > 0 && pwd[i - 1] != '/')
			i--;
		prompt = ft_strdup(pwd + i);
	}
	prompt = ft_join_free_s2("\001\033[1;32m\002", prompt);
	prompt = ft_join_free_s1(prompt, "\001\033[1;34m\002 $> \001\033[0m\002");
	free(pwd);
	free(var->prompt);
	var->prompt = prompt;
}
