/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:33:04 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 09:42:22 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define EXPORT 1
#define UNSET 0

void	ft_env(char **args, t_env *env)
{
	if (ft_strequ_hd(args[0], "env"))
		ft_putlst(env->envp);
}


int	manag_exec_in_env(t_list *cmd)
{
	char	**args;
	char	**new_args;
	int		i;

	i = 0;
	new_args = NULL;
	args = ((t_cmd *)cmd->content)->args;
	if (!args || !args[0])
		return (0);
	if (!ft_strequ_hd(args[0], "env"))
		return (0);
	while (ft_strequ_hd(args[i], "env") && args[i - 1])
		i++;
	if (check_the_build_for_env(args[i]))
		return (1);
	new_args = ft_dupdpt(&args[i]);
	ft_free_split(args);
	((t_cmd *)cmd->content)->args = new_args;
	return (0);	
}

int	ft_pwd(char **args)
{
	char	*pwd;

	// no need apparamment
	//if (args[1])
	//	return (error(ft_strdup("pwd"), "too many arguments", BUILD_ERROR));
	(void)args;
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}


int	ft_unset(char **arg, t_env *env_set)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (arg[++i])
	{
		if (is_valide_var(arg[i], UNSET))
		{
			if (ft_strchr(arg[i], '='))
			{
				delref(&(env_set->envp), ft_strjoin(arg[i], "="));
				delref(&(env_set->ex_env), ft_strjoin(arg[i], "="));
			}
			else
				delref(&(env_set->ex_env), ft_strdup(arg[i]));
		}
		else
		{
			print_error(ft_strdup("unset"), "not a valid identifier");
			ret = BUILD_ERROR;
		}
	}
	return (ret);
}


int	ft_export(char **arg, t_env *env_set)
{
	int	i;
	int	equ;
	int	ret;

	i = 0;
	equ = 0;
	ret = 0;
	if (!arg[1])
		; // ft_putexport(env_set->ex_env); what the hell joseph
	else
	{
		while (arg[++i])
		{
			if (is_valide_var(arg[i], EXPORT))
			{
				equ = ft_strc_index(arg[i], '=');
				if (equ != -1)
					add_ref(&(env_set->envp), arg[i], equ + 1);
				else
					add_ref(&(env_set->ex_env), arg[i], ft_strlen(arg[i]));
			}
			else
			{
				print_error(ft_strdup("export"), "not a valid identifier");
				ret = BUILD_ERROR;
			}
		}
	}
	return (ret);
}
