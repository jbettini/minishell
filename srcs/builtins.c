/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:32:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/26 20:08:17 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define	HOME 1
#define	OLD 2

int	ft_cmd(char **args, t_list	**env)
{
	if (args[0] == NULL)
		return (0);
	else if (ft_strequ_hd(args[0], "exit"))
		return (ft_exit_code(args, 1));
	else if (ft_strequ_hd(args[0], "unset"))
		ft_unset(args, env);
	else if (ft_strequ_hd(args[0], "export"))
		ft_export(args, env);
	else if (ft_strequ_hd(args[0], "pwd"))
		ft_pwd();
	else if (ft_strequ_hd(args[0], "echo"))
		ft_echo(args);
	else if (ft_strequ_hd(args[0], "cd"))
		ft_cd(args, env);
	else if (ft_strequ_hd(args[0], "env"))
		ft_env(args, env);
	else
		return (1);
	return (0);
}

void	cd_to_envvar(t_list **env, char *var)
{
	t_list	*tmp;
	char	*str;

	tmp = *env;
	str = NULL;
	while (tmp && !ft_strnequ(tmp->content, var, ft_strlen(var)))
		tmp = tmp->next;
	if (tmp)
		my_chdir(ft_del_nl(ft_strnstr_out(tmp->content, var, ft_strlen(var))), env);
	else
	{
		if (ft_strnequ("HOME=", var, ft_strlen(var)))
			ft_putendl_fd("cd: HOME not set", 2);
		else if (ft_strnequ("OLDPWD=", var, ft_strlen(var)))
			ft_putendl_fd("cd: OLDPWD not set", 2);
	}
}

void	my_chdir(char *path, t_list **env)
{
	char	*pwd;
	char	*oldpwd;
	char	*tmp;

	tmp = NULL;
	pwd = NULL;
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
		perror("cd");
	else
	{
		tmp = ft_strjoin("OLDPWD=", oldpwd);
		add_ref(env, tmp, 8);
		free(tmp);
		pwd = getcwd(NULL, 0);
		tmp = ft_strjoin("PWD=", pwd);
		add_ref(env, tmp, 5);
		free(pwd);
		free(tmp);
	}
	free(oldpwd);
}

void	ft_cd(char **args, t_list **env)
{
	if (ft_double_strlen(args) <= 2) // need change to 3 arg
	{
		if (ft_strequ_hd(args[1], "~") || !args[1])
			cd_to_envvar(env, "HOME=");
		else if (ft_strequ_hd(args[1], "-"))
			cd_to_envvar(env, "OLDPWD=");
		else			// need 1 more else if for 3 arg
			my_chdir(args[1], env);
	}
	else
		ft_putendl_fd("cd: too many arguments", 2);
}

void	exec_in_env(char **args, t_list **env, char **path)
{
	int		pid;
	char	*tmp;

	if (ft_isbuild(args[0]))
		tmp = NULL;
	else
		tmp = parse_cmd(path, args);
	pid = fork();
	if (!pid)
	{
		if (ft_cmd(args, env))
			execve(tmp, args, NULL);
		exit(1);
	}
	else
	{
		waitpid(-1, NULL, 0);
		if (tmp)
			free(tmp);
	}
}

void	ft_env(char **args, t_list **env)
{
	char	**path;
	char	*tmp;

	tmp = getenv("PATH");
	path = ft_split(tmp, ':');
	if (!args[1])
		ft_putlst(*env);
	else
		exec_in_env(&args[1], env, path);
	// free(tmp);
	ft_free_split(path);
}