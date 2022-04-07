/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:32:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/07 19:44:03 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define HOME 1
#define OLD 2

void	cd_to_envvar(t_list **env, char *var)
{
	t_list	*tmp;
	char	*str;

	tmp = *env;
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
	if (!arg[1] || (!arg[2] && ft_is_str_blank(arg[1])))
		return (ft_putchar_fd('\n', 1));
	while (arg[++i])
	{
		ft_putstr_fd(arg[i], 1);
		if (arg[i + 1])
			ft_putchar_fd(' ', 1);
	}
	if (!ft_strequ_hd(arg[1], "-n"))
		ft_putchar_fd('\n', 1);
}
