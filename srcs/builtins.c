/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:32:07 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/24 18:50:41 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cmd(char **args, t_list	**env)
{
	if (args[0] == NULL)
		return (0);
	else if (ft_strequ_hd(args[0], "exit"))
		return (ft_exit_code(args, 1));
	else if (ft_strequ_hd(args[0], "env"))
		ft_env(env);
	else if (ft_strequ_hd(args[0], "unset"))
		ft_check_unset(args, env);
	else if (ft_strequ_hd(args[0], "export"))
		ft_check_export(args, env);
	else if (ft_strequ_hd(args[0], "pwd"))
		ft_pwd();
	else if (ft_strequ_hd(args[0], "cd"))
		ft_cd(args, env);
	else if (ft_strequ_hd(args[0], "echo"))
		ft_echo(args);
	else
		return (1);
	return (0);
}

void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	free(pwd);
}

void	ft_cd(char **args, t_list **env)
{
	char	*oldpwd;

	if (args[1] == NULL || ft_is_str_blank(args[1]))
		my_chdir(env, ft_strdup(getenv("HOME")));
	else if (ft_strequ_hd(args[1], "-"))
	{
		oldpwd = ft_my_getenv(env, "OLDPWD=");
		if (oldpwd)
			my_chdir(env, ft_strdup(oldpwd));
		else
			ft_putendl_fd("cd: OLDPWD not set", 2);
	}
	else
		my_chdir(env, ft_get_path(args[1]));
}

void	ft_write_oldpwd(t_list **env, char *pwd)
{
	t_list	*tmp_env;

	tmp_env = *env;
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->content, "OLDPWD=", 7) == 0)
		{
			free(tmp_env->content);
			tmp_env->content = ft_strjoin("OLDPWD=", pwd);
		}
		tmp_env = tmp_env->next;
	}
}

char	*ft_my_getenv(t_list **env, char *key)
{
	t_list	*tmp_env;

	tmp_env = *env;
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->content, key, ft_strlen(key)) == 0)
			return (ft_strchr(tmp_env->content, '=') + 1);
		tmp_env = tmp_env->next;
	}
	return (NULL);
}


void	ft_env(t_list **env)
{
	char	*tmp;
	char	*cwd;
	t_list	*tmp_env;

	tmp_env = *env;
	cwd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", cwd);
	free(cwd);
	cwd = ft_strjoin(tmp, "\n");
	free(tmp);
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->content, "PWD=", 4) == 0)
		{
			free(tmp_env->content);
			tmp_env->content = cwd;
			ft_putlst(*env);
			return ;
		}
		tmp_env = tmp_env->next;
	}
	ft_lstadd_back(env, ft_lstnew(cwd));
	ft_putlst(*env);
}
