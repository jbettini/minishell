/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:33:04 by jbettini          #+#    #+#             */
/*   Updated: 2022/05/03 18:39:01 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define EXPORT 1
#define UNSET 0

void	ft_env(char **args, t_env *env)
{
	(void)args;
	ft_putlst(env->envp);
}

int	ft_pwd(char **args)
{
	char	*pwd;

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
			delref(&(env_set->envp), ft_strjoin(arg[i], "="));
			delref(&(env_set->ex_env), ft_strjoin(arg[i], "="));
		}
		else
		{
			print_error(ft_strdup("unset"), "not a valid identifier");
			ret = BUILD_ERROR;
		}
	}
	return (ret);
}

void	nex(t_list **envp, t_list **ex, char *arg, int equ)
{
	add_ref(envp, arg, equ, 0);
	add_ref(ex, arg, ft_strlen(arg), 0);
}

int	ft_export(char **arg, t_env *env)
{
	t_cpt	t;

	init_cpt(&t);
	if (!arg[1])
		ft_putexport(env->ex_env);
	else
	{
		while (arg[++(t.i)])
		{
			if (is_valide_var(arg[t.i], EXPORT))
			{
				t.equ = ft_strc_index(arg[t.i], '=');
				if (t.equ != -1)
					nex(&(env->envp), &(env->ex_env), arg[t.i], t.equ + 1);
				else
					add_ref(&(env->ex_env), arg[t.i], ft_strlen(arg[t.i]), 1);
			}
			else
			{
				print_error(ft_strdup("export"), "not a valid identifier");
				t.ret = BUILD_ERROR;
			}
		}
	}
	return (t.ret);
}
