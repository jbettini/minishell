/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:21:35 by rahmed            #+#    #+#             */
/*   Updated: 2022/02/13 20:36:04 by rahmed           ###   ########.fr       */
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
	char	*path;
	char	*tmp;
	t_list	*tmp_env;

	path = NULL;
	tmp_env = *env;
	if (args[1] == NULL || ft_is_str_blank(args[1]))
		chdir(getenv("HOME"));
	else
	{
		while (tmp_env)
		{
			if (ft_strncmp(tmp_env->content, "OLDPWD=", 7) == 0)
			{
				free(tmp_env->content);
				tmp = getcwd(NULL, 0);
				path = ft_strjoin(tmp, "\n");
				free(tmp);
				tmp_env->content = ft_strjoin("OLDPWD=", path);
				free(path);
			}
			tmp_env = tmp_env->next;
		}
		freed_chdir(ft_get_path(args[1]));
	}
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
