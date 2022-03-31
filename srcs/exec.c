/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 14:38:36 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/31 19:34:18 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isbuild(char *args)
{
	if (ft_strequ_hd(args, "exit"))
		return (1);
	else if (ft_strequ_hd(args, "env"))
		return (1);
	else if (ft_strequ_hd(args, "unset"))
		return (1);
	else if (ft_strequ_hd(args, "export"))
		return (1);
	else if (ft_strequ_hd(args, "pwd"))
		return (1);
	else if (ft_strequ_hd(args, "cd"))
		return (1);
	else if (ft_strequ_hd(args, "echo"))
		return (1);
	return (0);
}

int	execute_cmd(char **args, t_env *env, int mod)
{
	int	ret;
	
	ret = 0;
	if (mod == IN_ENV)
		ret = exec_build_in_env(args, env);
	else if (mod != IN_MAIN)
		ret = ft_cmd(args, env, mod);
	else if (mod == IN_MAIN)
		ret = 2;
	if (ret == 2 && !env->cmd_path)
	{
		ret = CMD_ERROR;
		error_manag(ret);
	}
	else if (ret == 2)
		execve(env->cmd_path, args, env->nbtfke);
	exit(ret);
}

int	check_unset_path(char **path, t_env *env)
{
	int	i;

	i = 0;
	while (path[i])
	{
		if (ft_strequ_hd(path[i], "PATH"))
			break ;
		i++;
	}
	if (path[i])
	{
		ft_free_split(env->path);
		env->path = NULL;	
	}
	return (0);
}

int	exec_in_main(t_cmd *cmd, t_env *env, int mod)
{
	int	ret;

	ret = 0;
	if (mod == IN_MAIN)
	{
		ret = ft_cmd(cmd->args, env, mod);
		if (ret != 2)
			g_exit_status = ret;
		else if (ret == 0 && ft_strequ_hd(cmd->args[0], "unset"))
			check_unset_path(&cmd->args[1], env);
		else if (ret == 2 && !env->cmd_path)
		{
			error_manag(CMD_ERROR);
			g_exit_status = CMD_ERROR;
		}
		else if (ret == 2)
			exec_in_child(cmd->args, env, mod);		
	}
	else if (mod == IN_PIPE)
	{
		ret = redir_lst(cmd->redir_out, env);
		if (ret)
			exit(1);
		execute_cmd(cmd->args, env, mod);
	}
	return (ret);
}

char	**remake_path(t_env *env, char **args)
{
	int	i;

	i = 0;
	if (ft_strequ_hd(args[0], "env"))
	{
		while (ft_strequ_hd(args[i], "env") && args[i + 1])
			i++;
		if (env->cmd_path)
			free(env->cmd_path);
		set_path(env, &args[i], SET);
		return (&args[i]);
	}
	return (NULL);
}

int	exec_in_child(char **args, t_env *env, int mod)
{
	int		pid;
	char	**exec_args;

	exec_args = remake_path(env, args);
	if (!exec_args)
		exec_args = args;
	pid = fork();
	if (!pid)
	{
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		execute_cmd(exec_args, env, mod);
	}
	else
		env->child++;
	return (0);
}

char	*parse_cmd(char **path, char **cmd)
{
	char	*cmd_path;
	char	*tmp;
	size_t	i;

	if (!path)
		return (NULL);
	i = -1;
	tmp = ft_strjoin("/", cmd[0]);
	while (path[++i])
	{
		cmd_path = ft_strjoin(path[i], tmp);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free(tmp);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free(tmp);
	return (NULL);
}
