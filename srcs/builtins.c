/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:32:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 19:02:12 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define HOME 1
#define OLD 2

void	cd_to_envvar(t_env *env, char *var)
{
	t_list	*tmp;
	char	*str;

	tmp = env->envp;
	str = NULL;
	while (tmp && !ft_strnequ(tmp->content, var, ft_strlen(var)))
		tmp = tmp->next;
	if (tmp)
		my_chdir(ft_del_nl(ft_strnstr_out(tmp->content, \
								var, ft_strlen(var))), env);
	else
	{
		if (ft_strnequ("HOME=", var, ft_strlen(var)))
			ft_putendl_fd("cd: HOME not set", 2);
		else if (ft_strnequ("OLDPWD=", var, ft_strlen(var)))
			ft_putendl_fd("cd: OLDPWD not set", 2);
	}
}

void	my_chdir(char *path, t_env *env)
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
		add_ref(&env->envp, tmp, 8, 0);
		add_ref(&env->ex_env, tmp, 8, 0);
		free(tmp);
		pwd = getcwd(NULL, 0);
		tmp = ft_strjoin("PWD=", pwd);
		add_ref(&env->ex_env, tmp, 5, 0);
		add_ref(&env->envp, tmp, 5, 0);
		free(pwd);
		free(tmp);
	}
	free(oldpwd);
}

void	ft_cd(char **args, t_env *env)
{
	if (!args[1])
		cd_to_envvar(env, "HOME=");
	else if (ft_strequ_hd(args[1], "-"))
		cd_to_envvar(env, "OLDPWD=");
	else
		my_chdir(args[1], env);
}

void	ft_echo(char **arg)
{
	int	i;

	i = ft_strequ_hd(arg[1], "-n");
	while (arg[++i])
	{
		ft_putstr_fd(arg[i], 1);
		if (arg[i + 1])
			ft_putchar_fd(' ', 1);
	}
	if (!ft_strequ_hd(arg[1], "-n"))
		ft_putchar_fd('\n', 1);
}
