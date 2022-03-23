/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:03:54 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/23 07:53:16 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*env_manag(char **env, t_env *to_free, int mod)
{
	t_env	*env_set;

	if (mod)
	{
		ft_lstclear(&(to_free->envp), free);
		ft_free_split(to_free->nbtfke);
		if (to_free->path)
			ft_free_split(to_free->path);
		free(to_free);
		return (0);
	}
	env_set = malloc(sizeof(t_env));
	if (!env_set)
		return (NULL);
	env_set->child = 0;
	env_set->oldstdout = dup(1);
	env_set->oldstdin = dup(0);
	env_set->envp = ft_dpt_to_lst(env);
	env_set->nbtfke = ft_lst_to_dpt(env_set->envp);
	env_set->path = ft_split(getenv("PATH"), ':');
	if (!(env_set->envp) || !(env_set->path))
		return (NULL);
	return (env_set);
}

void wait_this_fk_process(t_env *env)
{
	int	i;
	int status;

	i = -1;
	status = 0;
	// printf("child %d\n", env->child);
	if (env->child)
	{
		while (++i < env->child)
			waitpid(-1, &status, 0);
		g_exit_status = status % 255;
	}
}

int	minishell(t_env *env_set)
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
		cmds = parse(line);
		if (cmds)
		{
			if (!expand_ev(cmds, env_set))
				; // smth bad occurred
			ret = connecting_fct(cmds, env_set);
		}
		ft_lstclear(&cmds, &free_cmd);
	}
	free(line);
	return (ret);
}

int	ft_exit(char **args, int print_exit)
{
	if (print_exit)
		ft_putstr_fd("exit\n", 1);
	if (args[1] && !ft_str_isdigit(args[1]))
	{
		print_error("exit: numeric argument required");
		exit(255);
	}
	else if (ft_double_strlen(args) > 2)
		print_error("exit: too many arguments");
	else
	{
		if (args[1])
			exit(ft_atoll(args[1]));
		exit(0);
	}
	return (BUILD_ERROR);
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

