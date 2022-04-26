/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 04:17:11 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/26 18:40:37 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_heredoc(char *stop)
{
	char	**hd;
	int		fd;
	int		fd2;
	int		i;

	i = -1;
	fd = open(".heredoc_tmp", O_CREAT | O_RDWR | O_TRUNC, 0744);
	if (fd == -1)
		return (OP_ERROR);
	hd = here_doc(stop);
	if (!hd)
		return (CTRL_C);
	while (hd[++i])
		ft_putendl_fd(hd[i], fd);
	close(fd);
	fd2 = open(".heredoc_tmp", O_RDONLY);
	if (dup2(fd2, 0) == -1)
		return (DUP_ERROR);
	close(fd2);
	ft_free_split(hd);
	return (SUCCESS);
}

char	**here_doc(char *stop)
{
	char	**tab;
	char	*rd_ret;
	t_list	*lst;

	rd_ret = NULL;
	lst = NULL;
	g.in_hd = 1;
	g.hd_exited_from_sigint = 0;
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		rd_ret = ft_get_next_line(STDIN_FILENO);
		if (g.hd_exited_from_sigint)
		{
			g.in_hd = 0;
			return (NULL);
		}
		if (rd_ret && rd_ret[0])
			rd_ret = ft_str_del_nl(rd_ret);
		if (!rd_ret || ft_strequ_hd(rd_ret, stop))
		{
			g.in_hd = 0;
			free(rd_ret);
			break ;
		}
		ft_lstadd_back(&lst, ft_lstnew(ft_strdup(rd_ret)));
		free(rd_ret);
	}
	tab = ft_lst_to_dpt(lst);
	ft_lstclear(&lst, &free);
	return (tab);
}

int	redir_to_stdout(void *file, int mod)
{
	int	fd;

	fd = 0;
	if (file == NULL || ft_strequ_hd(file, "|"))
		return (OUT_ERROR);
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) == -1)
			return (PERM_ERROR);
	if (mod == O_TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0744);
	else if (mod == O_APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0744);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 1) == -1)
		return (DUP_ERROR);
	close(fd);
	return (0);
}

int	redir_to_stdin(void *file)
{
	int	fd;

	if (access(file, F_OK) == -1)
		return (BF_ERROR);
	if (access(file, R_OK) == -1)
		return (PERM_ERROR);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 0) == -1)
		return (DUP_ERROR);
	close(fd);
	return (0);
}
