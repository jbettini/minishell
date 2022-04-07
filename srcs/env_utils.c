/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:33:04 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/04 22:33:24 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define EXPORT 1
#define UNSET 0

int	is_valide_var(char *str, int mod)
{
	int	i;

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

int	ft_strc_index(char *str, int c)
{
	size_t	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (i);
	}
	return (-1);
}

void	delref(t_list **lst, void *data_ref)
{
	t_list	*tmp;
	t_list	*before;

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

void	add_ref(t_list **lst, void *data_ref, int idx)
{
	t_list	*tmp;

	tmp = *lst;
	while (tmp && !ft_strnequ(tmp->content, data_ref, idx))
		tmp = tmp->next;
	if (!tmp)
		ft_lstadd_back(lst, ft_lstnew(ft_strdup(data_ref)));
	else
	{
		free(tmp->content);
		tmp->content = NULL;
		tmp->content = ft_strdup(data_ref);
	}
}

void	ft_putexport(t_list *lst)
{
	while (lst)
	{
		ft_putstr("declare -x ");
		ft_putendl_fd(lst->content, 2);
		lst = lst->next;
	}
}
