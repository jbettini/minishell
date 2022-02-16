/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:39 by ydanset           #+#    #+#             */
/*   Updated: 2022/02/16 03:23:20 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// #include "cmds.h"

// # define PROMPT "minishell> "

/*
	to do :
		- make a custom prompt?
		- research potential parsing errors and handle it
		- consider the fact that readline doesn't read tabulation (am i right?)

	must ensure that :
		- ev name must not start by a digit >> OK PATCH
		- ev name consist of alpha, numeric and/or '_' characters >> OK PATCH
		- ev name must not be empty > deja OK JE PRENDS PAS LES ARGS NULL
*/

// int	main(int ac, char **av, char **env)
// {
// 	char	*line;
// 	char	**local_env;
// 	t_list	*cmds;

// 	local_env = copy_strs(env);
// 	if (!local_env)
// 		exit_error("cannot allocate memory", EXIT_FAILURE);
// 	line = readline(PROMPT);
// 	while (line)
// 	{
// 		expand_ev(&line, local_env);
// 		cmds = parse(line);
// 		ft_lstiter(cmds, &print_cmd);
// 		if (cmds)
// 			; // execute commands
// 		ft_lstclear(&cmds, &free_cmd);
// 		free(line);
// 		line = readline(PROMPT);
// 	}
// 	free_strs(local_env);
// 	return (0);
// }


int	main(int ac, char **av, char **env)
{
	char	**local_env;
	t_env	*env_set;

	(void)ac;
	(void)av;
	env_set = env_manag(env, NULL, 0);
	local_env = copy_strs(env);
	init_signal(0);
	if (!local_env)
		exit_error("cannot allocate memory", EXIT_FAILURE);
	while (1)
		if (routine(local_env, env_set) == -1)
			break ;
	free_strs(local_env);
	env_manag(NULL, env_set, 1);
	// ft_printf("\033[1;31m****EXIT STATUS = %d****\033[0m\n", g_exit_status); //! test
	return (g_exit_status);
}


