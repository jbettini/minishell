/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:15 by ydanset           #+#    #+#             */
/*   Updated: 2022/04/03 13:10:05 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_word(char **word, t_var *var)
{
	int		in_double_quotes;
	int		i;

	in_double_quotes = 0;
	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '"' && !in_double_quotes)
			in_double_quotes = 1;
		else if ((*word)[i] == '"' && in_double_quotes)
			in_double_quotes = 0;
		if ((*word)[i] == '$' && ((*word)[i + 1] == '?'
			|| ft_isalpha((*word)[i + 1]) || (*word)[i + 1] == '_'))
			rearrange_word(word, &i, var);
		else if ((*word)[i] == '\'' && !in_double_quotes)
		{
			i++;
			while ((*word)[i] != '\'')
				i++;
			i++;
		}
		else
			i++;
	}
}

char	**expand_args(char **args, t_var *var)
{
	int		i;
	int		j;
	char	**new_args;
	char	**arg_expanded;

	new_args = NULL;
	i = -1;
	while (args && args[++i])
	{
		expand_word(&args[i], var);
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

int	expand_redir(t_redir *redir, t_var *var)
{
	char	*tmp;

	if (redir->type == REDIR_LL)
		return (0);
	tmp = ft_strdup(redir->filename);
	expand_word(&redir->filename, var);
	if (!redir_expanded_is_valid(redir->filename))
	{
		print_error(ft_strdup(tmp), "ambiguous redirect");
		free(tmp);
		return (1);
	}
	delete_quotes(&redir->filename);
	free(tmp);
	return (0);
}
