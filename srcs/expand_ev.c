/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbettini <jbettini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:15 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 03:15:52 by jbettini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_word(char **word, char **env)
{
	int		i;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '$')
		{
			if ((*word)[i + 1] == '?' || ft_isalpha((*word)[i + 1]) \
												|| (*word)[i + 1] == '_')
				rearrange_word(word, &i, env);
			else
				i++;
		}
		else if ((*word)[i++] == '\'')
			while ((*word)[i] && (*word)[i] != '\'')
				i++;
	}
}

char	**expand_args(char **args, char **env)
{
	int		i;
	int		j;
	char	**new_args;
	char	**arg_expanded;

	new_args = NULL;
	i = -1;
	while (args && args[++i])
	{
		expand_word(&args[i], env);
		if (!args[i][0])
			continue ;
		arg_expanded = ft_strtok(args[i], " \t\n");
		j = -1;
		while (arg_expanded[++j])
			delete_quotes(&arg_expanded[j]);
		new_args = strs_join(new_args, arg_expanded);
	}
	free_strs(args);
	return (new_args);
}

int	redir_expanded_is_valid(char *word_expanded)
{
	char	**tmp;

	tmp = ft_strtok(word_expanded, " \t\n");
	if (strs_len(tmp) != 1)
	{
		free_strs(tmp);
		return (0);
	}
	free_strs(tmp);
	return (1);
}

int	expand_redir(t_list *redirs, char **env)
{
	t_redir	*redir;

	while (redirs)
	{
		redir = redirs->content;
		if (redir->type != REDIR_LL)
		{
			expand_word(&redir->word, env);
			if (!redir_expanded_is_valid(redir->word))
				return (error("ambiguous redirect", 0));
			delete_quotes(&redir->word);
		}
		redirs = redirs->next;
	}
	return (1);
}

int	expand_ev(t_list *cmds, t_env *env)
{
	t_cmd	*cmd;
	char	**envp;

	envp = ft_lst_to_dpt(env->envp);
	while (cmds)
	{
		cmd = cmds->content;
		cmd->args = expand_args(cmd->args, envp);
		if (!expand_redir(cmd->redir_in, envp)
			|| !expand_redir(cmd->redir_out, envp))
		{
			free_strs(envp);
			return (0);
		}
		cmds = cmds->next;
	}
	free_strs(envp);
	return (1);
}
