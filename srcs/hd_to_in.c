/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_to_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:33:48 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/26 12:02:35 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unlink_all(t_env *env)
{
	t_list	*tmp;

	tmp = env->hd_to_unlink;
	while (tmp)
	{
		if (!access(tmp->content, F_OK))
			unlink(tmp->content);
		tmp = tmp->next;
	}
	ft_lstclear(&(env->hd_to_unlink), &free);
	env->hd_to_unlink = NULL;
}

int	no_bad_file(t_list *r_in)
{
	t_redir	*tmp;
	int		hd;

	hd = 0;
	while (r_in)
	{
		tmp = (t_redir *)r_in->content;
		if (tmp->type == REDIR_LL && hd != 1)
			hd++;
		r_in = r_in->next;
	}
	return (hd);
}

int	convert_a_hd(t_redir *redir, char *name)
{
	char	**hd;
	int		fd;
	int		i;

	i = -1;
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0744);
	if (fd == -1)
		return (OP_ERROR);
	hd = here_doc(redir->word);
	if (!hd)
	{
		write(1, "\n", 1);
		return (CTRL_C);
	}
	while (hd[++i])
		ft_putendl_fd(hd[i], fd);
	redir->type = REDIR_L;
	free((redir->word));
	redir->word = ft_strdup(name);
	close(fd);
	ft_free_split(hd);
	return (SUCCESS);
}

int	convert_all_hd(t_list *r_in, int i, t_env *env)
{
	char	*name;

	name = ft_join_free_ss(ft_join_free_s1(getcwd(NULL, 0), "/"), \
							ft_join_free_s2(".heredoc_tmp", ft_itoa(i)));
	ft_lstadd_back(&(env->hd_to_unlink), ft_lstnew(ft_strdup(name)));
	while (r_in)
	{
		if (((t_redir *)r_in->content)->type == REDIR_LL)
		{
			if (convert_a_hd(r_in->content, name) == CTRL_C)
			{
				free(name);
				return (CTRL_C);
			}
		}
		r_in = r_in->next;
	}
	free(name);
	return (SUCCESS);
}

int	hd_to_infile(t_list *cmds, t_env *env)
{
	int	i;

	i = 0;
	while (cmds)
	{
		if (no_bad_file(((t_cmd *)cmds->content)->redir_in))
			if (convert_all_hd(((t_cmd *)cmds->content)->redir_in, \
														i, env) == CTRL_C)
				return (CTRL_C);
		i++;
		cmds = cmds->next;
	}
	return (SUCCESS);
}
