#include "minishell.h"

#define EXPORT 1
#define UNSET 0

void	ft_env(char **args, t_var *var)
{
	(void)args;
	ft_putlst(var->local_env);
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

int	ft_unset(char **arg, t_var *var)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (arg[++i])
	{
		if (is_valide_var(arg[i], UNSET))
		{
			delref(&(var->local_env), ft_strjoin(arg[i], "="));
			delref(&(var->local_export), ft_strjoin(arg[i], "="));
		}
		else
		{
			print_error(ft_strdup("unset"), "not a valid identifier");
			ret = BUILD_ERROR;
		}
	}
	return (ret);
}

void	nex(t_list **local_env, t_list **ex, char *arg, int equ)
{
	add_ref(local_env, arg, equ, 0);
	add_ref(ex, arg, ft_strlen(arg), 0);
}

int	ft_export(char **arg, t_var *var)
{
	t_cpt	t;

	init_cpt(&t);
	if (!arg[1])
		ft_putexport(var->local_export);
	else
	{
		while (arg[++(t.i)])
		{
			if (is_valide_var(arg[t.i], EXPORT))
			{
				t.equ = ft_strc_index(arg[t.i], '=');
				if (t.equ != -1)
					nex(&(var->local_env), &(var->local_export), arg[t.i], t.equ + 1);
				else
					add_ref(&(var->local_export), arg[t.i], ft_strlen(arg[t.i]), 1);
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
