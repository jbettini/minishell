/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:33:48 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 04:32:26 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* need review wait_this_fk_process() */
t_env	*env_manag(char **env, t_env *to_free, int mod)
{
	t_env	*env_set;

	if (mod)
	{
		ft_lstclear(&(to_free->envp), free);
		ft_lstclear(&(to_free->ex_env), free);
		ft_free_split(to_free->nbtfke);
		if (to_free->path)
			ft_free_split(to_free->path);
		free(to_free);
		close(to_free->oldstdin);
		close(to_free->oldstdout);
		return (0);
	}
	env_set = malloc(sizeof(t_env));
	if (!env_set)
		return (NULL);
	env_set->child = 0;
	g_set.g_exit_status = 0;
	env_set->envp = ft_dpt_to_lst(env);
	env_set->ex_env = ft_dpt_to_lst(env);
	env_set->nbtfke = ft_lst_to_dpt(env_set->envp);
	env_set->path = ft_split(getenv("PATH"), ':');
	if (!(env_set->envp) || !(env_set->path))
		return (NULL);
	return (env_set);
}

void	wait_this_fk_process(t_env *env)
{
	int	i;
	int	status;
	int	x;

	x = 0;
	i = -1;
	status = 0;
	if (env->child)
	{
		while (++i < env->child)
		{
			waitpid(-1, &status, 0);
			if (WIFSIGNALED(status) && !x)
			{
				x = 1;		
				if (WTERMSIG(status) == SIGQUIT)
					write(STDOUT_FILENO, "Quit: 3", 7);
				write(STDOUT_FILENO, "\n", 1);
				g_set.g_exit_status = 128 + WTERMSIG(status);
			}
		}
		if (!x)
			g_set.g_exit_status = status % 255;
	}
}

int	minishell(t_env *env_set)
{
	int		ret;
	char	*line;
	t_list	*cmds;

	ret = 0;
	set_sig(SIGQUIT, SIG_IGN);
	set_sig(SIGINT, &sigint_handler);
	line = readline(PROMPT);
	env_set->oldstdout = dup(1);
	env_set->oldstdin = dup(0);
	if (!line)
	{
		env_manag(NULL, env_set, 1);
		reset_tty();
		ft_putstr_fd("exit\n", 1);
		exit(0);
	}
	else if (!ft_is_str_blank(line) && line)
	{
		add_history(line);
		cmds = parse(line);
		if (cmds)
		{
			set_sig(SIGINT, SIG_IGN);
			ret = connecting_fct(cmds, env_set);
		}
		ft_lstclear(&cmds, &free_cmd);
	}
	free(line);
	return (ret);
}

int	ft_exit(char **args, int print_exit, t_env *env_set)
{
	if (print_exit)
		ft_putstr_fd("exit\n", 1);
	if (args[1] && !ft_str_isdigit(args[1]))
	{
		print_error("exit: numeric argument required");
		env_manag(NULL, env_set, 1);
		exit(255);
	}
	else if (ft_double_strlen(args) > 2)
		print_error("exit: too many arguments");
	else
	{
		reset_tty();
		if (args[1])
		{
			env_manag(NULL, env_set, 1);
			system("leaks minishell");
			exit(ft_atoll(args[1]));
		}
		env_manag(NULL, env_set, 1);
		system("leaks minishell");
		exit(0);
	}
	return (BUILD_ERROR);
}

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
	return (g_set.g_exit_status);
}
