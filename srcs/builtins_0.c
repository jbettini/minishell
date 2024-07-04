/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:32:07 by jbettini          #+#    #+#             */
/*   Updated: 2024/07/04 02:43:00 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_to_envvar(t_var *var, char *ev_name)
{
	t_list	*tmp;

	tmp = var->local_env;
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

void	update_oldpwd(t_var *var)
{
	char	*ev_value;
	char	**env;
	char	*new_oldpwd;

	env = ft_lst_to_dpt(var->local_env);
	ev_value = get_ev_value("PWD", env);
	new_oldpwd = ft_strjoin("OLDPWD=", ev_value);
	free(ev_value);
	add_ref(&var->local_env, new_oldpwd);
	add_ref(&var->local_export, new_oldpwd);
	free(new_oldpwd);
	free_strs(env);
}

void	my_chdir(char *path, t_var *var)
{
	char	*pwd;
	char	*tmp;

	(void)var;
	tmp = NULL;
	pwd = NULL;
	if (chdir(path) == -1)
		perror("cd");
	else
	{
		update_oldpwd(var);
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			print_error(ft_strdup("cd"), "No such file or directory");
			return ;
		}
		tmp = ft_strjoin("PWD=", pwd);
		add_ref(&var->local_export, tmp);
		add_ref(&var->local_env, tmp);
		free(pwd);
		free(tmp);
	}
}

void	ft_cd(char **args, t_var *var)
{
	if (!args[1])
		cd_to_envvar(var, "HOME=");
	else if (ft_strequ_hd(args[1], "-"))
		cd_to_envvar(var, "OLDPWD=");
	else
		my_chdir(args[1], var);
	set_prompt(var);
}
