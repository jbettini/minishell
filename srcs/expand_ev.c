/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydanset <ydanset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 14:26:15 by ydanset           #+#    #+#             */
/*   Updated: 2022/03/31 19:07:34 by ydanset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_ev_name(char *str)
{
	int		i;
	char	*ev_name;

	if (str[0] == '?')
		return (ft_strdup("?"));
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	ev_name = malloc(i + 1);
	my_strncpy(ev_name, str, i);
	return (ev_name);
}

static char	*get_ev_value(char *ev_name, char **env)
{
	char	*ev_value;
	char	*tmp;
	int		i;

	i = 0;
	while (env[i])
	{
		tmp = get_ev_name(env[i]);
		if (!my_strcmp(tmp, ev_name))
		{
			ev_value = get_str_truncated(env[i], 0, ft_strlen(ev_name) + 1);
			free(tmp);
			return (ev_value);
		}
		free(tmp);
		i++;
	}
	ev_value = ft_strdup("");
	return (ev_value);
}

static void	rearrange_word(char **word, int *i, char **env)
{
	char	*ev_name;
	char	*ev_value;

	ev_name = get_ev_name(&(*word)[*i + 1]);
	*word = trunc_str(*word, *i + 1, ft_strlen(ev_name));
	if (!my_strcmp(ev_name, "?"))
		ev_value = ft_itoa(g_exit_status);
	else
		ev_value = get_ev_value(ev_name, env);
	*word = str_insert(*word, ev_value, *i);
	*i += ft_strlen(ev_value);
	free(ev_value);
	free(ev_name);
}

void	expand_word(char **word, char **env)
{
	int		i;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '$')
		{
			if ((*word)[i + 1] == '?' || ft_isalpha((*word)[i + 1]) || (*word)[i + 1] == '_')
				rearrange_word(word, &i, env);
			else
				i++;
		}
		else if ((*word)[i++] == '\'')
			while ((*word)[i] && (*word)[i] != '\'')
				i++;
	}
}

void	delete_quotes(char **word)
{
	int		i;
	char	quote;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'' || (*word)[i] == '"')
		{
			quote = (*word)[i];
			*word = trunc_str(*word, i, 1);
			while ((*word)[i] && (*word)[i] != quote)
				i++;
			if ((*word)[i])
				*word = trunc_str(*word, i, 1);
		}
		else
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
