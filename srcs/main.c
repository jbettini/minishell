/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:33:48 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 17:31:56 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* need review wait_this_fk_process() */
void	*env_free(t_env *to_free)
{
	ft_lstclear(&(to_free->envp), free);
	ft_lstclear(&(to_free->ex_env), free);
	ft_free_split(to_free->nbtfke);
	if (to_free->path)
		ft_free_split(to_free->path);
	close(to_free->oldstdin);
	close(to_free->oldstdout);
	free(to_free);
	reset_usr_tty_config(to_free);
	return (0);
}

t_env	*env_manag(char **env, t_env *to_free, int mod)
{
	t_env	*env_set;

	if (mod)
		return (env_free(to_free));
	env_set = malloc(sizeof(t_env));
	env_set->child = 0;
	env_set->cmd_path = NULL;
	env_set->last_pid = 0;
	g.exit_status = 0;
	env_set->hd_to_unlink = NULL;
	env_set->envp = ft_dpt_to_lst(env);
	env_set->ex_env = ft_dpt_to_lst(env);
	env_set->nbtfke = ft_lst_to_dpt(env_set->envp);
	env_set->path = ft_split(getenv("PATH"), ':');
	if (!(env_set->envp) || !(env_set->path))
		return (NULL);
	save_usr_tty_config(env_set);
	return (env_set);
}

int	minishell(t_env *env_set)
{
	int		ret;
	char	*line;
	t_list	*cmds;

	ret = 0;
	set_sig(SIGINT, &sigint_handler);
	line = readline(PROMPT);
	env_set->oldstdout = dup(1);
	env_set->oldstdin = dup(0);
	if (!line)
		return (cette_fct_seet_a_normer_minishell());
	else if (!ft_is_str_blank(line) && line)
	{
		add_history(line);
		cmds = parse(line);
		if (cmds)
			ret = exec_cmds(cmds, env_set);
		ft_lstclear(&cmds, &free_cmd);
	}
	free(line);
	return (ret);
}

int	main(int ac, char **av, char **env)
{
	t_env	*env_set;

	(void)ac;
	(void)av;
	env_set = env_manag(env, NULL, 0);
	set_sig(SIGQUIT, SIG_IGN);
	while (1)
	{
		tty_hide_ctrl();
		if (minishell(env_set) == -1)
			break ;
	}
	env_manag(NULL, env_set, 1);
	return (g.exit_status);
}
