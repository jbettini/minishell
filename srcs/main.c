/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:33:48 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/02 20:24:52 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* need review wait_this_fk_process() */

int	main(int ac, char **av, char **env)
{
	t_env	*env_set;

	(void)ac;
	(void)av;
	env_set = env_manag(env, NULL, 0);
	set_tty();
	while (1)
		if (minishell(env_set) == -1)
			break ;
	env_manag(NULL, env_set, 1);
	system("leaks minishell");
	return (g_exit_status);
}
