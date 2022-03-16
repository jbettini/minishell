/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 14:38:36 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/16 04:02:51 by jbettini         ###   ########.fr       */
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
	if (mod)
	{
		ret = ft_cmd(cmd->args, &(env->envp));
		if (!ret && ft_strequ_hd(cmd->args[0], "unset"))
			check_unset_path(&cmd->args[1], env);
		if (ret > 0)
			ret = exec_in_child(cmd, env);
	}
	else
	{
		ret = redir_lst(cmd->redir_out, env);
		if (ret)
			exit(-42);
		if (!env->cmd_path)
			ret = CMD_ERROR;
		if (ft_cmd(cmd->args, &(env->envp)) > 0)
			execve((env->cmd_path), cmd->args, NULL);
		exit(0);
	}
	return (ret);
}

int	exec_in_child(t_cmd	*cmd, t_env *env)
{
	int	pid;

	if (ft_strchr(cmd->args[0], '/'))
		env->cmd_path = ft_strdup(cmd->args[0]);
	else
		env->cmd_path = parse_cmd(env->path, cmd->args);
	pid = fork();
	if (!pid)
	{
		if (ft_cmd(cmd->args, &(env->envp)) > 0)
			execve((env->cmd_path), cmd->args, NULL);
		exit(0);
	}
	else
	{
		waitpid(-1, NULL, 0);
		free(env->cmd_path);
	}
	if (!env->cmd_path)
		return (CMD_ERROR);
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
