/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:33:04 by jbettini          #+#    #+#             */
/*   Updated: 2022/03/30 16:34:09 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

# define EXPORT 1
# define UNSET 0

int is_valide_var(char *str, int mod)
{
    int i;

    i = 0;
    if (str[0] != '_' && !ft_isalpha(str[0]))
        return (0);
    if (mod == EXPORT)
    {
        while (str[++i] && str[i] != '=')
            if (!ft_isalnum(str[i]) && str[i] != '_')
                return (0);
    }
    else if (mod == UNSET)
    {
        while (str[++i])
            if (!ft_isalnum(str[i]) && str[i] != '_')
                return (0);
    }
    return (1);
}

int ft_strc_index(char *str, int c)
{
    size_t  i;

    i = -1;
    while (str[++i])
    {
        if (str[i] == c)
            return (i);
    }
    return (-1);
}

int	ft_pwd(char **args)
{
	char	*pwd;

    if (args[1])
    {
        print_error("pwd: too many arguments");
        return (BUILD_ERROR);
    }
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	free(pwd);
    return (0);
}

void    delref(t_list **lst, void *data_ref)
{
    t_list  *tmp;
    t_list  *before;

    tmp = *lst;
    before = NULL;
    if (ft_strnequ(tmp->content, data_ref, ft_strlen(data_ref)))
    {
        *lst = (*lst)->next;
        free(tmp->content);
        free(tmp);
        return ;
    }
    while (tmp && !ft_strnequ(tmp->content, data_ref, ft_strlen(data_ref)))
    {
        before = tmp;
        tmp = tmp->next;
    }
    if (tmp)
    {
        before->next = tmp->next;
        free(tmp->content);
        free(tmp);
    }
    free(data_ref);
}

int	ft_unset(char **arg, t_env *env_set)
{
    int i;
    int ret;

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
            ft_putstr_fd("unset:", 2);
            ft_putstr_fd(arg[i], 2);
            ft_putstr_fd(" : invalid parameter name\n", 2);
            ret = BUILD_ERROR;
        }
    }
    return (ret);
}

void    add_ref(t_list **lst, void *data_ref, int idx)
{
    t_list  *tmp;

    tmp = *lst;
    while (tmp && !ft_strnequ(tmp->content, data_ref, idx))
        tmp = tmp->next;
    if (!tmp)
        ft_lstadd_back(lst, ft_lstnew(ft_strjoin(data_ref, "\n")));
    else
    {
        free(tmp->content);
        tmp->content = NULL;
        tmp->content = ft_strjoin(data_ref, "\n");
    }
}

void	ft_putexport(t_list *lst)
{
	while (lst)
	{
		ft_putstr("declare -x ");
		ft_putstr(lst->content);
		lst = lst->next;
	}
}

int	ft_export(char **arg, t_env *env_set)
{
    int i;
    int equ;
    int ret;

    i = 0;
    equ = 0;
    ret = 0;
    if (!arg[1])
        ft_putexport(env_set->ex_env);
    else
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
                ft_putstr_fd("export: not valid in this context: ", 2);
                ft_putendl_fd(arg[i], 2);
                ret = BUILD_ERROR;
            }
        }
    return (ret);
}