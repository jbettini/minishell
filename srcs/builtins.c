/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:32:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/31 12:49:35 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define	HOME 1
#define	OLD 2

int	ft_cmd(char **args, t_env *env, int mod)
{
	if (args[0] == NULL)
		return (0);
	else if (ft_strequ_hd(args[0], "exit") && mod != IN_ENV)
		return (ft_exit(args, 1, env));
	else if (ft_strequ_hd(args[0], "unset") && mod != IN_ENV)
		return (ft_unset(args, env));
	else if (ft_strequ_hd(args[0], "export") && mod != IN_ENV)
		return (ft_export(args, env));
	else if (ft_strequ_hd(args[0], "pwd"))
		return (ft_pwd(args));
	else if (ft_strequ_hd(args[0], "echo"))
		ft_echo(args);
	else if (ft_strequ_hd(args[0], "cd"))
		ft_cd(args, &(env->envp));
	else if (ft_strequ_hd(args[0], "env"))
		ft_env(args, env, mod);
	else
		return (2);
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
		if (!args[1])
			cd_to_envvar(env, "HOME=");
		else if (ft_strequ_hd(args[1], "-"))
			cd_to_envvar(env, "OLDPWD=");
		else			// need 1 more else if for 3 arg
			my_chdir(args[1], env);
	}
	else
		ft_putendl_fd("cd: too many arguments", 2);
}

int	exec_build_in_env(char **args, t_env *env)
{
	int ret;

	ret = CMD_ERROR;
	if (ft_strequ_hd(args[0], "unset"))
		ft_putstr("env: export: No such file or directory\n");
	else if (ft_strequ_hd(args[0], "export"))
		ft_putstr("env: export: No such file or directory\n");
	else if (ft_strequ_hd(args[0], "exit"))
		ft_putstr("env: exit: No such file or directory\n");
	else
		ret = 0;
	if (ret == 0)
		ret = ft_cmd(args, env, IN_ENV);
	return (ret);
}

void	ft_env(char **args, t_env *env, int mod)
{
	int		i;

	i = 0;
	while (ft_strequ_hd(args[i], "env") && args[i + 1])
		i++;
	if (ft_strequ_hd(args[i], "env"))
		ft_putlst(env->envp);
	else
	{
		if (mod == IN_MAIN)
		{
			if (env->cmd_path)
				free(env->cmd_path);
			set_path(env, &args[i], SET);
			exec_in_child(&args[i], env, IN_ENV);
		}
	}
}