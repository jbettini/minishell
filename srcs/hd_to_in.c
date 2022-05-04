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

void	unlink_all(t_var *var)
{
	t_list	*tmp;

	tmp = var->hd_to_unlink;
	while (tmp)
	{
		if (!access(tmp->content, F_OK))
			unlink(tmp->content);
		tmp = tmp->next;
	}
	ft_lstclear(&(var->hd_to_unlink), &free);
	var->hd_to_unlink = NULL;
}

int	convert_a_hd(t_redir *redir)
{
	char	*line;
	int		fd;

	fd = open(redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0744);
	if (fd == -1)
	{
		print_error(NULL, strerror(errno));
		return (OP_ERROR);
	}
	g.in_hd = 1;
	g.sigint_in_hd = 0;
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line_hd(STDIN_FILENO);
		if (g.sigint_in_hd || !line || !my_strcmp(redir->keyword, line))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	g.in_hd = 0;
	if (g.sigint_in_hd)
		return (CTRL_C);
	close(fd);
	return (SUCCESS);
}

int	convert_all_hd(t_list *redirs, int i, t_var *var)
{
	while (redirs)
	{
		if (((t_redir *)redirs->content)->type == REDIR_LL)
		{
			((t_redir *)redirs->content)->filename = ft_join_free_ss(ft_join_free_s1(getcwd(NULL, 0), "/"), \
							ft_join_free_s2(".heredoc_tmp", ft_itoa(i)));
			ft_lstadd_back(&(var->hd_to_unlink), ft_lstnew(ft_strdup(((t_redir *)redirs->content)->filename)));
			if (convert_a_hd(redirs->content) == CTRL_C)
				return (CTRL_C);
		}
		redirs = redirs->next;
	}
	return (SUCCESS);
}

int	hd_to_infile(t_list *cmds, t_var *var)
{
	int	i;

	i = 0;
	while (cmds)
	{
			if (convert_all_hd(((t_cmd *)cmds->content)->redirs, \
														i, var) == CTRL_C)
				return (CTRL_C);
		i++;
		cmds = cmds->next;
	}
	return (SUCCESS);
}
