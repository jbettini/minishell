/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniparser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahmed <rahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 06:41:32 by jbettini          #+#    #+#             */
/*   Updated: 2022/02/13 19:10:47 by rahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strequ_h(char const *s1, char const *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	is_notredir(char *tab)
{
	if (ft_strequ_h(tab, "<"))
		return (0);
	else if (ft_strequ_h(tab, "<<"))
		return (0);
	else if (ft_strequ_h(tab, ">"))
		return (0);
	else if (ft_strequ_h(tab, ">>"))
		return (0);
	return (1);
}

int	tmp_utils(t_cmd **cmd, char **str, int mod)
{
	int	i;

	i = 0;
	if (!mod)
	{
		// (*cmd)->name = NULL;
		(*cmd)->args = NULL;
		(*cmd)->redir_in = NULL;
		(*cmd)->redir_out = NULL;
	}
	else
		while (str[i] && is_notredir(str[i]))
			i++;
	return (i);
}

t_list	*redir(char *tmp, char *file)
{
	t_redir	*redir;

	(void) tmp; // !to silent warning
	redir = malloc(sizeof(t_redir));
	// redir->type = ft_strdup(tmp); //TODO  a adapter avec les tokens
	redir->word = ft_strdup(file);
	return (ft_lstnew(redir));
}

// void	free_redir(void *ptr)
// {
// 	t_list	*lst;
// 	t_redir	*redir;

// 	lst = (t_list *)ptr;
// 	redir = (t_redir *)lst->content;
// 	// free(redir->type);
// 	free(redir->word);
// 	free(redir);
// }

void	destroy_biglst_content(t_list **lst)
{
	t_list	*tmp;
	t_cmd	*cmd;

	tmp = *lst;
	while (tmp)
	{
		cmd = (t_cmd *)tmp->content;
		ft_lstclear(&(cmd->redir_in), free_redir);
		ft_lstclear(&(cmd->redir_out), free_redir);
		// free(cmd->name);
		ft_free_split(cmd->args);
		tmp = tmp->next;
	}
}

int	cmd_parser(t_cmd **cmd, char **tab)
{
	int	i;

	i = 0;
	while (tab[i] && is_notredir(tab[i]))
		i++;
	(*cmd)->args = malloc(sizeof(char *) * i + 1);
	((*cmd)->args)[i] = NULL;
	i = -1;
	while (tab[++i] && is_notredir(tab[i]))
		((*cmd)->args)[i] = ft_strdup(tab[i]);
	return (i);
}

t_list	*cmd_maker(char *str)
{
	t_list	*lst;
	t_cmd	*tmp;
	char	**tab;
	int		i;
	int		k;

	if (!str)
		return (NULL);
	i = -1;
	k = 0;
	tab = ft_split(str, ' ');
	tmp = malloc(sizeof(t_cmd));
	tmp_utils(&tmp, NULL, 0);
	lst = NULL;
	while (tab[++i])
	{
		if (ft_strequ_h(tab[i], "<") && ++i)
			ft_lstadd_back(&(tmp->redir_in), redir(tab[i - 1], tab[i]));
		else if (ft_strequ_h(tab[i], "<<") && ++i)
			ft_lstadd_back(&(tmp->redir_in), redir(tab[i - 1], tab[i]));
		else if (ft_strequ_h(tab[i], ">") && ++i)
			ft_lstadd_back(&(tmp->redir_in), redir(tab[i - 1], tab[i]));
		else if (ft_strequ_h(tab[i], ">>") && ++i)
			ft_lstadd_back(&(tmp->redir_in), redir(tab[i - 1], tab[i]));
		else
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(tab[i])));
	}
	ft_free_split(tab);
	tmp->args = ft_lst_to_dpt(lst);
	ft_lstclear(&lst, free);
	// tmp->name = tmp->args[0];
	return (ft_lstnew(tmp));
}

t_list	*miniparser(char *str)
{
	int		i;
	t_list	*lst;
	char	**cmd_list;

	i = -1;
	lst = NULL;
	cmd_list = ft_split(str, '|');
	while (cmd_list[++i])
		ft_lstadd_back(&lst, cmd_maker(cmd_list[i]));
	ft_free_split(cmd_list);
	return (lst);
}

void	ft_print_dpt(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		printf("arg %d : %s\n", i, split[i]);
}

void	print_redir_lst(t_list *lst)
{
	t_redir	*redir;

	while (lst)
	{
		redir = (t_redir *)lst->content;
		// printf("type : %s\tfilename : %s\n", redir->type, redir->filename);
		printf("type : %d\tname : %s\n", redir->type, redir->word);
		lst = lst->next;
	}
}

void	ft_putbiglst(t_list *lst)
{
	t_cmd	*cmd;

	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		print_redir_lst(cmd->redir_in);
		print_redir_lst(cmd->redir_out);
		ft_print_dpt(cmd->args);
		lst = lst->next;
	}
}

// /* PAs de leaks alors que je free pas la BIGGGGG Lst Bizarrrrrrrrrrrrrrrrrrrrrrrrrre :S */

// int main(int argc, char **argv)
// {
// 	t_list	*lst = miniparser(argv[1]);
// 	ft_putbiglst(lst);
	// // destroy_biglst_content(&lst);
// 	system("leaks a.out");
// }