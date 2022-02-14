/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 04:17:11 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/14 01:34:26 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redir_heredoc(char *stop)
{
	char	**hd;
	int		fd;
	int		fd2;
	int		i;

	i = -1;
	fd = open(".heredoc_tmp", O_CREAT | O_RDWR | O_TRUNC, 644);
	if (fd == -1)
		return (OP_ERROR);
	hd = here_doc(stop);
	while (hd[++i])
		ft_putendl_fd(hd[i], fd);
	close(fd);
	usleep(2000);
	fd2 = open(".heredoc_tmp", O_RDONLY);
	if (dup2(fd2, 0) == -1)
		return (DUP_ERROR);
	ft_free_split(hd);
	return (SUCCESS);
}

char	**here_doc(char *stop)
{
	char	**tab;
	char	*rd_ret;
	t_list	*lst;
	int		check;

	check = 1;
	rd_ret = NULL;
	lst = NULL;
	while (check == 1)
	{
		rd_ret = readline("heredoc> ");
		if (!ft_strequ_hd(rd_ret, stop))
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(rd_ret)));
		else
			check--;
		free(rd_ret);
	}
	tab = ft_lst_to_dpt(lst);
	ft_lstclear(&lst, free);
	return (tab);
}

int	redir_to_stdout(void *file, int mod)
{
	int	fd;

	fd = 0;
	if (file == NULL || ft_strequ_hd(file, "|"))
		return (OUT_ERROR);
	if (mod == O_TRUNC)
		fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 644);
	else if (mod == O_APPEND)
		fd = open(file, O_CREAT | O_RDWR | O_APPEND, 644);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 1) == -1)
		return (DUP_ERROR);
	return (0);
}

int	redir_to_stdin(void *file)
{
	int	fd;

	if (access(file, F_OK) == -1)
		return (BF_ERROR);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 0) == -1)
		return (DUP_ERROR);
	return (0);
}
