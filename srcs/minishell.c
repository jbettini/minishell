/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:03:54 by rahmed            #+#    #+#             */
/*   Updated: 2022/02/16 07:58:21 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*env_manag(char **env, t_env *to_free, int mod)
{
	t_env	*env_set;

	if (mod)
	{
		ft_lstclear(&(to_free->envp), free);
		ft_free_split(to_free->path);
		free(to_free);
		return (0);
	}
	env_set = malloc(sizeof(t_env));
	if (!env_set)
		return (NULL);
	env_set->oldstdout = dup(1);
	env_set->oldstdin = dup(0);
	env_set->envp = ft_dpt_to_lst(env);
	env_set->path = ft_split(getenv("PATH"), ':');
	if (!(env_set->envp) || !(env_set->path))
		return (NULL);
	return (env_set);
}

int	routine(char **local_env, t_env *env_set)
{
	// TODO : get choice between char** and t_env
	int		ret;
	char	*line;
	t_list	*cmds;

	ret = 0;
	line = readline(PROMPT);
	if (handle_eof_ctrl_d(line))
		ret = -1;
	else if (!ft_is_str_blank(line) && line)
	{
		add_history(line);
		expand_ev(&line, local_env);
		cmds = parse(line);
		// ft_lstiter(cmds, &print_cmd);
		if (cmds)
			ret = connecting_fct(cmds, env_set); // ! t_env *env to send or char **
		ft_lstclear(&cmds, &free_cmd);
	}
	free(line);
	return (ret);
}

int	ft_exit_code(char **args, int print_exit)
{
	//! tester les valeurs exit merdiques et negatives par rapport au vrai bash
	int	valid_code;

	if (print_exit)
		ft_putstr_fd("exit\n", 1);
	if (args[1] == NULL)
		return (-1);
	valid_code = ft_str_isdigit(args[1]);
	if (!valid_code) // ! verif si > INT MAX aussi ?
		print_error("exit: numeric argument required");
	else if (args[2])
		print_error("exit: too many arguments");
	else if (valid_code && !args[2])
	{
		g_exit_status = ft_atoll(args[1]) % 256;
		// ft_printf("---------------SORTIE = %d\n", g_exit_status); // ! test
		return (-1);
	}
	return (0);
}

// int	routine(t_env *env_set)
// {
// 	char	*line;
// 	int		ret;
// 	t_list	*cmd;

// 	ret = 0;
// 	line = readline(PROMPT);
// 	if (handle_eof_ctrl_d(line))
// 		ret = -1;
// 	else if (!ft_is_str_blank(line) && line)
// 	{
// 		add_history(line);
// 		cmd = miniparser(line);
// 		ret = connecting_fct(cmd, env_set);
// 	}
// 	free(line);
// 	// ft_printf("0 routine ret = %d\n", ret); //! test
// 	return (ret);
// }

