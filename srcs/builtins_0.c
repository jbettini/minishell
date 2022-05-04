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

void	cd_to_envvar(t_var *var, char *ev_name)
{
	t_list	*tmp;
	char	*str;

	tmp = var->local_env;
	str = NULL;
	while (tmp && !ft_strnequ(tmp->content, ev_name, ft_strlen(ev_name)))
		tmp = tmp->next;
	if (tmp)
		my_chdir(ft_del_nl(ft_strnstr_out(tmp->content, \
								ev_name, ft_strlen(ev_name))), var);
	else
	{
		if (ft_strnequ("HOME=", ev_name, ft_strlen(ev_name)))
			ft_putendl_fd("cd: HOME not set", 2);
		else if (ft_strnequ("OLDPWD=", ev_name, ft_strlen(ev_name)))
			ft_putendl_fd("cd: OLDPWD not set", 2);
	}
}

void	my_chdir(char *path, t_var *var)
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
		add_ref(&var->local_env, tmp, 8, 0);
		add_ref(&var->local_export, tmp, 8, 0);
		free(tmp);
		pwd = getcwd(NULL, 0);
		tmp = ft_strjoin("PWD=", pwd);
		add_ref(&var->local_export, tmp, 5, 0);
		add_ref(&var->local_env, tmp, 5, 0);
		free(pwd);
		free(tmp);
	}
	free(oldpwd);
}

void	ft_cd(char **args, t_var *var)
{
	if (!args[1])
		cd_to_envvar(var, "HOME=");
	else if (ft_strequ_hd(args[1], "-"))
		cd_to_envvar(var, "OLDPWD=");
	else
		my_chdir(args[1], var);
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
