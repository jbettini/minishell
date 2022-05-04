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

void	*free_var(t_var *to_free)
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

t_var	*var_manager(char **env, t_var *to_free, int mode)
{
	t_var	*var;

	if (mode)
		return (free_var(to_free));
	var = malloc(sizeof(t_var));
	var->child = 0;
	var->cmd_path = NULL;
	var->last_pid = 0;
	g.exit_status = 0;
	var->hd_to_unlink = NULL;
	var->envp = ft_dpt_to_lst(env);
	var->ex_env = ft_dpt_to_lst(env);
	var->nbtfke = ft_lst_to_dpt(var->envp);
	var->path = ft_split(getenv("PATH"), ':');
	if (!(var->envp) || !(var->path))
		return (NULL);
	save_usr_tty_config(var);
	return (var);
}

int	minishell(t_var *var)
{
	int		ret;
	char	*line;
	t_list	*cmds;

	ret = 0;
	set_sig(SIGINT, &sigint_handler);
	line = readline(PROMPT);
	var->oldstdout = dup(1);
	var->oldstdin = dup(0);
	if (!line)
	{
		ft_putstr_fd("exit\n", 1);
		return (-1);
	}
	else if (line && !ft_is_str_blank(line))
	{
		add_history(line);
		cmds = parse(line);
		if (cmds)
			ret = exec_cmds(cmds, var);
		ft_lstclear(&cmds, &free_cmd);
	}
	free(line);
	return (ret);
}

// check leaks
// check unlink
// check exit status after ctrl+C in hd
// what to do when redir all returns an error
// norme
int	main(int ac, char **av, char **env)
{
	t_var	*var;

	(void)ac;
	(void)av;
	var = var_manager(env, NULL, 0);
	set_sig(SIGQUIT, SIG_IGN);
	while (1)
	{
		tty_hide_ctrl();
		if (minishell(var) == -1)
			break ;
	}
	var_manager(NULL, var, 1);
	return (g.exit_status);
}
