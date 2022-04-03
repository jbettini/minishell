/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 04:17:11 by jbettini          #+#    #+#             */
/*   Updated: 2022/04/03 08:54:58 by ydanset          ###   ########.fr       */
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
	{
		write(1, "\n", 1);
		return (CTRL_C);
	}
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
	int		check;

	rd_ret = NULL;
	lst = NULL;
	check = 1;
	set_sig(SIGINT, &sig_hd_handler);
	while (check)
	{
		rd_ret = readline("> ");
		if (g_set.g_check_hd)
		{
			set_sig(SIGINT, SIG_IGN);
			ft_lstclear(&lst, &free);
			return (NULL);
		}
		if (!rd_ret || ft_strequ_hd(rd_ret, stop))
			check--;
		else
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(rd_ret)));
		free(rd_ret);
	}
	set_sig(SIGINT, SIG_IGN);
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
	if (mod == O_TRUNC)
		fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0744);
	else if (mod == O_APPEND)
		fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0744);
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
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (OP_ERROR);
	if (dup2(fd, 0) == -1)
		return (DUP_ERROR);
	close(fd);
	return (0);
}

void	ft_pipex(t_cmd *cmd, t_env *env)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (!pid)
	{
		set_sig(SIGINT, SIG_DFL);
		set_sig(SIGQUIT, SIG_DFL);
		dup2(fd[1], 1);
		close(fd[0]);
		exec_in_main(cmd, env, IN_PIPE);
	}
	else
	{
		env->child++;
		dup2(fd[0], 0);
		close(fd[1]);
		close(fd[0]);
	}
}